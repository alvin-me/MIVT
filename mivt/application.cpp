#include "application.h"
#include "offscreenrender.h"
#include "rendervolume.h"
#include "logmanager.h"
#include "filesystem.h"
#include "gpucapabilities.h"
#include "shadermanager.h"
#include "matrixstack.h"
#include "rawvolumereader.h"
#include "volume.h"
#include "transfunc1d.h"

#include <iostream>

namespace mivt {

  const std::string Application::loggerCat_ = "Application";

  Application::Application()
    : offscreen_(0)
    , logManager_(0)
    , volume_(0)
    , transfunc_(0)
    , render_(0)
  {
    Initialize();
  }

  Application::~Application()
  {
    Deinitialize();
  }

  void Application::Initialize()
  {
    ///
    /// 1. initialize none opengl parameters
    ///

    // detect base path based on program location
    programPath_ = tgt::FileSystem::currentDirectory();
    basePath_ = ".";
    // cut path from program location
    std::string::size_type p = programPath_.find_last_of("/\\");
    if (p != std::string::npos) {
      basePath_ = programPath_.substr(0, p);
    }
    // try to find base path starting at program path
    basePath_ = tgt::FileSystem::findWithSubDir(basePath_, "resource/transfuncs", 7);
    if (basePath_ == "") {
      std::cerr << "WARNING: Base path not found. Using current directory instead.\n";
      basePath_ = ".";
    }
    basePath_ = tgt::FileSystem::absolutePath(basePath_);
    userDataPath_ = tgt::FileSystem::cleanupPath(getBasePath("userdata"));

    tgt::LogManager::init();
    tgt::FileSystem::init();

    initLogging();

    ///
    /// 2. create render context.
    ///

    offscreen_ = new tgt::OffScreenRender();

    ///
    /// 3. initialize opengl parameters
    ///

    tgt::GpuCapabilities::init();
    tgt::ShaderManager::init();
    tgt::MatrixStack::init();

    // init shader path
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(getBasePath("mivt/glsl")));
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(getBasePath("mivt/glsl/base")));
    ShdrMgr.addPath(tgt::FileSystem::cleanupPath(getBasePath("mivt/glsl/modules")));

    render_ = new RenderVolume();
    render_->Initialize();

    initTransfunc();
  }

  void Application::Deinitialize()
  {
    render_->Deinitialize();
    DELPTR(render_);

    DELPTR(volume_);

    tgt::GpuCapabilities::deinit();
    tgt::ShaderManager::deinit();
    tgt::MatrixStack::deinit();

    tgt::FileSystem::deinit();
    tgt::LogManager::deinit();

    DELPTR(offscreen_);
  }

  void Application::GetPixels(unsigned char* buffer, int length) 
  {
    render_->GetPixels(buffer, length);
    //memset(buffer, 0, length * sizeof(unsigned char));
  }

  void Application::Resize(int width, int height) 
  {
    render_->Resize(glm::ivec2(width, height));
  }

  std::string Application::getBasePath(const std::string& filename) const {
    return tgt::FileSystem::cleanupPath(basePath_ + (filename.empty() ? "" : "/" + filename));
  }

  std::string Application::getProgramPath() const {
    return programPath_;
  }

  std::string Application::getUserDataPath(const std::string& filename) const {
    return tgt::FileSystem::cleanupPath(userDataPath_ + (filename.empty() ? "" : "/" + filename));
  }

  std::string Application::getResourcePath(const std::string& filename) const {
    return tgt::FileSystem::cleanupPath(getBasePath() + "/resource" + (filename.empty() ? "" : "/" + filename));
  }

  void Application::initLogging() {
    if (!tgt::Singleton<tgt::LogManager>::isInited()) {
      return;
    }

    LogMgr.clear();

    // HTML file logging
    std::string htmlLogFile = "mivt-log.html";
    std::string absLogPath;

    // add HTML file logger
    tgt::Log* log = 0;
    if (tgt::FileSystem::isAbsolutePath(htmlLogFile)) {
      absLogPath = htmlLogFile;
    }
    else {
      LogMgr.setLogDir(getUserDataPath()); //< write log file to user data dir by default
      absLogPath = tgt::FileSystem::absolutePath(LogMgr.getLogDir() + "/" + htmlLogFile);
    }
    log = new tgt::HtmlLog(htmlLogFile, false, true, true, true, false);
    assert(log);

    log->addCat("", true, tgt::LogLevel::Info);
    LogMgr.addLog(log);

    htmlLogFile = absLogPath;
  }

  void Application::Rotate(int newPosX, int newPosY, int lastPosX, int lastPosY)
  {
    render_->Rotate(glm::ivec2(newPosX, newPosY), glm::ivec2(lastPosX, lastPosY));
  }

  void Application::Zoom(int newPosX, int newPosY, int lastPosX, int lastPosY)
  {
    render_->Zoom(glm::ivec2(newPosX, newPosY), glm::ivec2(lastPosX, lastPosY));
  }

  void Application::Pan(int newPosX, int newPosY, int lastPosX, int lastPosY)
  {
    render_->Pan(glm::ivec2(newPosX, newPosY), glm::ivec2(lastPosX, lastPosY));
  }

  void Application::LoadVolume(const std::string &fileName,
    const std::string& format,
    const int dimension[3],
    const float spacing[3],
    float intercept,
    float slope,
    float windowWidth,
    float windowCenter)
  {
    try {
      tgt::RawVolumeReader reader;
      reader.setReadHints(glm::ivec3(dimension[0], dimension[1], dimension[2]),
        glm::vec3(spacing[0], spacing[1], spacing[2]),
        format, 0, false,
        intercept,
        slope,
        windowCenter,
        windowWidth);
      DELPTR(volume_);
      volume_ = reader.read(fileName);
      if (volume_) {
        tgt::oldVolumePosition(volume_);
      }
    }
    catch (const tgt::FileException& e) {
      LERROR(e.what());
    }
    catch (std::bad_alloc&) {
      LERROR("bad allocation while reading file: " << fileName);
    }

    if (volume_) {
      render_->SetVolume(volume_);
    }
  }

  void Application::initTransfunc()
  {
    transfuncName_ = "Vascular_Leg_Runoff";
    transfunc_ = new tgt::TransFunc1D();
    transfunc_->setToStandardFunc();
    transfunc_->load(getResourcePath("transfuncs") + "\\" + transfuncName_ + ".xml");

    render_->SetTransfunc(transfunc_);
  }

  void Application::SetTransfunc(const std::string& fileName)
  {
    transfuncName_ = fileName;
    transfunc_->load(getResourcePath("transfuncs") + "\\" + fileName + ".xml");
  }

  std::string Application::GetTransfunc()
  {
    return transfuncName_;
  }

  void Application::SetClassificationMode(const std::string& mode)
  {
    render_->SetClassificationMode(mode);
  }

  std::string Application::GetClassificationMode()
  {
    return render_->GetClassificationMode();
  }
}