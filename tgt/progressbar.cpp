#include "progressbar.h"
#include "logmanager.h"

namespace tgt {

  std::string ProgressBar::loggerCat_ = "ProgressBar";

  ProgressBar::ProgressBar()
    : progress_(0)
    , progressRange_(0.f, 1.f)
    , printedErrorMessage_(false)
  {}

  void ProgressBar::setProgress(float progress) {
    if (!(progressRange_.x <= progressRange_.y && progressRange_.x >= 0.f && progressRange_.y <= 1.f)) {
      LERROR("invalid progress range");
    }

    if (progress < 0.f) {
      if (!printedErrorMessage_)
        LWARNING("progress value " << progress << " out of valid range [0,1]");
      printedErrorMessage_ = true;
      progress = 0.f;
    }
    else if (progress > 1.f) {
      if (!printedErrorMessage_)
        LWARNING("progress value " << progress << " out of valid range [0,1]");
      printedErrorMessage_ = true;
      progress = 1.f;
    }
    else {
      printedErrorMessage_ = false;
    }

    progress_ = progressRange_.x + progress*(progressRange_.y - progressRange_.x);

    update();
  }

  float ProgressBar::getProgress() const {
    return progress_;
  }

  void ProgressBar::setProgressMessage(const std::string& message) {
    message_ = message;
    update();
  }

  std::string ProgressBar::getProgressMessage() const {
    return message_;
  }

  std::string ProgressBar::getMessage() const {
    return message_;
  }

  void ProgressBar::setTitle(const std::string& title) {
    title_ = title;
  }

  std::string ProgressBar::getTitle() const {
    return title_;
  }

  void ProgressBar::setProgressRange(const glm::vec2& progressRange) {
    if (!(progressRange.x <= progressRange.y && progressRange.x >= 0.f && progressRange.y <= 1.f)) {
      LERROR("invalid progress range");
    }
    progressRange_ = progressRange;
  }

  glm::vec2 ProgressBar::getProgressRange() const {
    return progressRange_;
  }


} // end namespace tgt
