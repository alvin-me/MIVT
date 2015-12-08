using System;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace mivtve
{
  class MyColorConverter
  {
    public static float[] StringToArray(string str)
    {
      var c = (Color)ColorConverter.ConvertFromString(str);
      float[] arr = new float[4];
      arr[0] = Convert.ToSingle(c.R) / 255.0f;
      arr[1] = Convert.ToSingle(c.G) / 255.0f;
      arr[2] = Convert.ToSingle(c.B) / 255.0f;
      arr[3] = Convert.ToSingle(c.A) / 255.0f;
      return arr;
    }

    public static string ArrayToString(float[] arr)
    {
      return Color.FromArgb(Convert.ToByte(arr[3] * 255), Convert.ToByte(arr[0] * 255),
        Convert.ToByte(arr[1] * 255), Convert.ToByte(arr[2] * 255)).ToString();
    }
  }

  public class PointCollectionConverter : IValueConverter
  {
    #region IValueConverter Members

    public object Convert(object value, System.Type targetType, object parameter, System.Globalization.CultureInfo culture)
    {
      if (value.GetType() == typeof(ObservableCollection<Point>) && targetType == typeof(PointCollection))
      {
        var pointCollection = new PointCollection();
        foreach (var point in value as ObservableCollection<Point>)
          pointCollection.Add(point);
        return pointCollection;
      }
      return null;
    }

    public object ConvertBack(object value, System.Type targetType, object parameter, System.Globalization.CultureInfo culture)
    {
      return null; //not needed
    }

    #endregion
  }
}
