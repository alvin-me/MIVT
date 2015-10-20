#pragma once
#include "volumederiveddata.h"

#include <vector>
#include <stdarg.h>
#include <stdint.h>
#include <cassert>

namespace tgt {

  class Volume;

  template<typename T, int ND>
  class HistogramGeneric {
  public:
    int getNumBuckets(int dim) const {
      if (dim >= 0 && dim < ND)
        return bucketCounts_[dim];
      else {
        assert(false);
        return 0;
      }
    }

    size_t getNumBuckets() const {
      return buckets_.size();
    }

    /// Returns the number of samples inserted into this histogram.
    uint64_t getNumSamples() const {
      return numSamples_;
    }

    /// Returns the number of samples in this bucket.
    uint64_t getBucket(size_t b) const {
      if (b < getNumBuckets())
        return buckets_[b];
      else {
        assert(false);
        return 0;
      }
    }

    uint64_t getBucket(size_t b1, size_t b2, ... /* size_t for each further dimension */) const {
      if (ND >= 2) {
        int c[ND];

        c[0] = static_cast<int>(b1);
        c[std::minus(1, ND - 1)] = static_cast<int>(b2); //Workaround to fix WARNING C4789 UNDER VC11

        va_list args;
        va_start(args, b2);
        for (int i = 2; i < ND; i++)
          c[i] = static_cast<int>(va_arg(args, size_t));
        va_end(args);

        int b = getBucketNumber(c);
        return getBucket(b);
      }
      else {
        assert(false);
        return 0;
      }
    }

    uint64_t getMaxBucket() const {
      //TODO: optimize?
      uint64_t max = 0;
      for (size_t i = 0; i<getNumBuckets(); i++)
      if (buckets_[i] > max)
        max = buckets_[i];

      return max;
    }

    /// Returns the normalized count in the bucket (getBucket(b) / getMaxBucket())
    float getBucketNormalized(int b) const {
      return (float)getBucket(b) / (float)getMaxBucket();
    }

    float getBucketLogNormalized(int b) const {
      return (logf(static_cast<float>(1 + getBucket(b))) / logf(static_cast<float>(1 + getMaxBucket())));
    }

    void increaseBucket(size_t b) {
      if (b < getNumBuckets()) {
        buckets_[b]++;
        numSamples_++;
      }
      else
        assert(false);
    }

    void increaseBucket(size_t bucket, uint64_t value) {
      if (bucket < getNumBuckets()) {
        buckets_[bucket] += value;
        numSamples_ += value;
      }
      else
        assert(false);
    }

    void decreaseBucket(size_t b) {
      if (b < getNumBuckets() && buckets_[b] > 0) {
        buckets_[b]--;
        numSamples_--;
      }
      else
        assert(false);
    }

    T getMinValue(int dim) const {
      return minValues_[dim];
    }

    T getMaxValue(int dim) const {
      return maxValues_[dim];
    }

  protected:
    void addSample(double sample, ... /* double sample for each further dimension */) {
      T values[ND];
      values[0] = static_cast<T>(sample);

      va_list args;
      va_start(args, sample);
      for (int i = 1; i<ND; i++) {
        values[i] = static_cast<T>(va_arg(args, double));
      }
      va_end(args);

      int c[ND];
      for (int i = 0; i<ND; i++)
        c[i] = mapValueToBucket(values[i], i);

      int b = getBucketNumber(c);
      increaseBucket(b);
    }

    int mapValueToBucket(T v, int dim) const {
      if (v < minValues_[dim]) {
        //TODO: out of range
        return 0;
      }
      else if (v > maxValues_[dim]) {
        //TODO: out of range
        return (bucketCounts_[dim] - 1);
      }
      else {
        v -= minValues_[dim];
        return static_cast<int>(bucketCounts_[dim] * (v / (maxValues_[dim] - minValues_[dim])));
        //TODO: clamp?
      }
    }

    int getBucketNumber(const int* c) const {
      int n = 0;
      int helper = 1;
      for (int i = 0; i<ND; i++) {
        if ((c[i] >= 0) && (c[i] < bucketCounts_[i])) {
          n += helper * c[i];
          helper *= bucketCounts_[i];
        }
        else {
          //TODO
        }
      }
      return n;
    }

    HistogramGeneric(double minValue, double maxValue, int bucketCount, ... /*, double minValue, double maxValue, int bucketCount for each further dimension */) : numSamples_(0) {
      va_list args;
      va_start(args, bucketCount);

      minValues_[0] = static_cast<T>(minValue);
      maxValues_[0] = static_cast<T>(maxValue);
      bucketCounts_[0] = bucketCount;

      for (int i = 1; i<ND; i++) {
        minValues_[i] = static_cast<T>(va_arg(args, double));
        maxValues_[i] = static_cast<T>(va_arg(args, double));
        bucketCounts_[i] = va_arg(args, int);
      }

      va_end(args);

      int numBuckets = 1;
      for (int i = 0; i<ND; i++)
        numBuckets *= bucketCounts_[i];

      buckets_.assign(numBuckets, 0);
    }

  private:
    T minValues_[ND];
    T maxValues_[ND];
    int bucketCounts_[ND];

    std::vector<uint64_t> buckets_;
    uint64_t numSamples_;
  };

  //------------------------------------------------------------------------------

  template <typename T>
  class Histogram1DGeneric : public HistogramGeneric<T, 1> {
  public:
    Histogram1DGeneric(T minValue, T maxValue, int bucketCount) : HistogramGeneric<T, 1>(static_cast<double>(minValue), static_cast<double>(maxValue), bucketCount) {}

    void addSample(T value) {
      HistogramGeneric<T, 1>::addSample(static_cast<double>(value));
    }
    T getMinValue() const {
      return HistogramGeneric<T, 1>::getMinValue(0);
    }

    T getMaxValue() const {
      return HistogramGeneric<T, 1>::getMaxValue(0);
    }
  private:
  };

  //------------------------------------------------------------------------------

  class Histogram1D : public Histogram1DGeneric<float> {
  public:
    Histogram1D(float minValue, float maxValue, int bucketCount) : Histogram1DGeneric<float>(minValue, maxValue, bucketCount) {}
    Histogram1D() : Histogram1DGeneric<float>(0.f, 1.f, 256) {}
  };

  //------------------------------------------------------------------------------

  Histogram1D createHistogram1DFromVolume(Volume *handle, size_t bucketCount, float realWorldMin, float realWorldMax);
  Histogram1D createHistogram1DFromVolume(Volume *handle, size_t bucketCount);

  //------------------------------------------------------------------------------

  template <typename T>
  class Histogram2DGeneric : public HistogramGeneric<T, 2> {
  public:
    Histogram2DGeneric(T minValue1, T maxValue1, int bucketCount1, T minValue2, T maxValue2, int bucketCount2) : HistogramGeneric<T, 2>(static_cast<double>(minValue1), static_cast<double>(maxValue1), bucketCount1, static_cast<double>(minValue2), static_cast<double>(maxValue2), bucketCount2) {}

    void addSample(T value1, T value2) {
      HistogramGeneric<T, 2>::addSample(static_cast<double>(value1), static_cast<double>(value2));
    }

  private:
  };

  //------------------------------------------------------------------------------

  class Histogram2D : public Histogram2DGeneric<float> {
  public:
    Histogram2D(float minValue1, float maxValue1, int bucketCount1, float minValue2, float maxValue2, int bucketCount2) : Histogram2DGeneric<float>(minValue1, maxValue1, bucketCount1, minValue2, maxValue2, bucketCount2) {}
  };

  //------------------------------------------------------------------------------

  //Histogram2D createHistogram2DFromVolume(Volume *handle, int bucketCountIntensity, int bucketCountGradient);

  //------------------------------------------------------------------------------

  /// 1D Intensity Histogram.
  class VolumeHistogramIntensity : public VolumeDerivedData {
  public:
    TGT_API VolumeHistogramIntensity(const Histogram1D& h);

    /// Empty default constructor required by VolumeDerivedData interface.
    TGT_API VolumeHistogramIntensity();

    /**
    * Creates a histogram with a bucket count of 256.
    *
    * @see VolumeDerivedData
    */
    TGT_API virtual VolumeDerivedData* createFrom(Volume* handle) const;

    /// Returns the number buckets of the specified channel histogram.
    TGT_API size_t getBucketCount() const;

    /// get value in bucket i
    TGT_API uint64_t getValue(int i) const;

    /// get value in bucket i
    TGT_API uint64_t getValue(size_t i) const;

    /// get value in bucket nearest to i
    TGT_API uint64_t getValue(float i) const;

    /// Returns normalized (with max.) histogram value at bucket i
    TGT_API float getNormalized(int i) const;

    /// Returns normalized (with max.) histogram value at bucket nearest to i
    TGT_API float getNormalized(float i) const;

    /// Returns normalized logarithmic histogram value at bucket i
    TGT_API float getLogNormalized(int i) const;

    /// Returns normalized logarithmic histogram value at bucket nearest to i
    TGT_API float getLogNormalized(float i) const;

    TGT_API const Histogram1D& getHistogram() const;
    TGT_API Histogram1D& getHistogram();

  protected:
    Histogram1D histogram_;
  };

} // end namespace tgt