#pragma once

#include "tgt_math.h"
#include "config.h"
#include <string>

namespace tgt {

  typedef void (__stdcall * ProgressCallback)(const char* msg);

  /**
  * Base class for GUI toolkit specific progress bars.
  */
  class ProgressBar {
  public:
    TGT_API ProgressBar(ProgressCallback callback);
    TGT_API virtual ~ProgressBar() {}

    /**
    * Makes the progress dialog visible.
    */
    TGT_API virtual void show();

    /**
    * Makes the progress dialog invisible.
    */
    TGT_API virtual void hide();

    /**
    * Calling this function is assumed to force a repaint,
    * rather than just schedule an update.
    */
    TGT_API virtual void forceUpdate();

    /// Override this method to get notified on IO progress.
    TGT_API virtual void update();

    /**
    * Assigns the current progress state.
    *
    * @param progress The current progress. Must
    * lie in [0, 1]
    */
    TGT_API virtual void setProgress(float progress);

    /**
    * Returns the current amount of progress
    * within the range [0, 1].
    */
    TGT_API virtual float getProgress() const;

    /**
    * Assigns a message that is to displayed by the
    * progress dialog.
    */
    TGT_API virtual void setProgressMessage(const std::string& message);

    /**
    * Returns the message that is to displayed by the
    * progress dialog.
    */
    TGT_API virtual std::string getProgressMessage() const;

    /// @overload
    TGT_API virtual std::string getMessage() const;

    /**
    * Assigns a title that is to displayed by the
    * progress dialog.
    */
    TGT_API virtual void setTitle(const std::string& title);

    /**
    * Assigns the range into which the progress value will be transformed,
    * i.e., actualProgress = progressRange.x + progress*(progressRange.y-progressRange.x)
    * The progress range must be a subrange of [0.f;1.f].
    * The default range is [0.f;1.f].
    */
    TGT_API virtual void setProgressRange(const glm::vec2& progressRange);

    TGT_API virtual glm::vec2 getProgressRange() const;

    /**
    * Returns the title that is to displayed by the
    * progress dialog.
    */
    TGT_API virtual std::string getTitle() const;

  protected:
    float progress_;
    std::string message_;
    std::string title_;

    glm::vec2 progressRange_;

    bool printedErrorMessage_;

    ProgressCallback callback_;

  private:
    static std::string loggerCat_;
  };

} // end namespace tgt
