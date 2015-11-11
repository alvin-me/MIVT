using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace mivtve
{
  public class MainWindowViewModel : ViewModel
  {
    #region Private Variables

    private mivtmanaged.Application _engine;
    private ImageSource             _imageSrc;
    private byte[]                  _imageBuffer;
    private WriteableBitmap         _imageBitmap;
    private bool                    _mouseTracking;
    private Point                   _lastMousePosition;
    private string                  _message;
    private string                  _messageBackup;
    private int                     _frameRate;
    private string                  _classificationMode;
    private int                     _protocalType;
    private List<ProtocalData>      _protocalList;
    private List<string>            _classificationModeList;
    private TransfuncWindow         _transfuncWindow = null;
    private LightWindow             _lightWindow = null;
    private float                   _shiness;
    private string                  _ambientColor;
    private string                  _diffuseColor;
    private string                  _specularColor;
    private List<string>            _bgColorModeList;
    private string                  _bgColorMode;
    private string                  _firstBgColor;
    private string                  _secondBgColor;
    private int                     _exportImageWidth;
    private int                     _exportImageHeight;
    private int                     _renderImageWidth;
    private int                     _renderImageHeight;

    [UnmanagedFunctionPointer(CallingConvention.StdCall)]
    delegate void ProgressCallback();

    #endregion

    #region Constructors

    public MainWindowViewModel()
    {
      _engine = new mivtmanaged.Application();

      _mouseTracking = false;

      // initialize Protocal type list 
      string basePath = System.AppDomain.CurrentDomain.BaseDirectory;
      basePath += "..\\..\\resource\\transfuncs\\";
      _protocalList = new List<ProtocalData>();
      foreach (var f in Directory.GetFiles(basePath))
      {
        string filename = Path.GetFileNameWithoutExtension(f);
        string photopath = Path.GetDirectoryName(f) + "\\thumbnails\\" + filename + ".bmp";
        if (!File.Exists(photopath))
          photopath = Path.GetDirectoryName(f) + "\\thumbnails\\custom.bmp";
        _protocalList.Add(new ProtocalData { Name = filename, Photo = photopath });
      }

      string protocal = _engine.GetTransfunc();
      for (int i = 0; i < _protocalList.Count; ++i )
      {
        if (_protocalList[i].Name == protocal)
        {
          ProtocalType = i;
          break;
        }
      }

      // initialize classificaiton mode list
      _classificationModeList = new List<string>();
      _classificationModeList.Add("transfer-function");
      _classificationModeList.Add("pre-integrated-cpu");
      _classificationModeList.Add("pre-integrated-gpu");

      ClassificationMode = _engine.GetClassificationMode();

      Shiness = _engine.GetMaterialShininess();

      float[] color = new float[4];
      _engine.GetLightAmbient(color);
      AmbientColor = MyColorConverter.ArrayToString(color);

      _engine.GetLightDiffuse(color);
      DiffuseColor = MyColorConverter.ArrayToString(color);

      _engine.GetLightSpecular(color);
      SpecularColor = MyColorConverter.ArrayToString(color);

      _bgColorModeList = new List<string>();
      _bgColorModeList.Add("none");
      _bgColorModeList.Add("monochrome");
      _bgColorModeList.Add("radial");
      _bgColorModeList.Add("gradient");

      BgColorMode = _engine.GetBgColorMode();

      _engine.GetFirstBgColor(color);
      FirstBgColor = MyColorConverter.ArrayToString(color);

      _engine.GetSecondBgColor(color);
      SecondBgColor = MyColorConverter.ArrayToString(color);

      // initialize commands

      ImageSizeChanged = new RelayCommand((x) =>
      {
        SizeChangedEventArgs args = (SizeChangedEventArgs)((Tuple<object, EventArgs>)x).Item2;
        int width = (int)args.NewSize.Width / 4 * 4;
        int height = (int)args.NewSize.Height;

        _imageBuffer = new byte[width * height * 4];

        // create bitmap for control binding
        PixelFormat pf = PixelFormats.Rgb24;
        int stride = (width * pf.BitsPerPixel + 7) / 8;
        _imageBitmap = new WriteableBitmap(width, height, 96, 96, pf, null);

        _engine.Resize(width, height);

        UpdateImage();

        // save for initialize export image size.
        _renderImageWidth = width;
        _renderImageHeight = height;
      });

      OpenSettingWindow = new RelayCommand((x) =>
      {
        var window = new SettingWindow();
        if (window.ShowDialog() == true)
        {

        }
      });

      OpenExportWindow = new RelayCommand((x) =>
      {
        var window = new ExportWindow();
        window.DataContext = this;

        ExportImageWidth = _renderImageWidth;
        ExportImageHeight = _renderImageHeight;

        if (window.ShowDialog() == true)
        {

        }
      });

      OpenTransfuncWindow = new RelayCommand((x) =>
      {
        if (_transfuncWindow == null)
        {
          _transfuncWindow = new TransfuncWindow();
          _transfuncWindow.DataContext = this;
        }
        _transfuncWindow.Show();
      });

      OpenLightWindow = new RelayCommand((x) =>
      {
        if (_lightWindow == null)
        {
          _lightWindow = new LightWindow();
          _lightWindow.DataContext = this;
        }
        _lightWindow.Show();
      });

      LoadVolume = new RelayCommand((x) =>
      {
        string ext = Path.GetExtension(Properties.Settings.Default.VolumeFile);
        if(ext == ".img" || ext == ".raw") {
          LoadRawVolume(Properties.Settings.Default.VolumeFile);
          UpdateImage();
          LogInfo("Volume Loaded.", 5000);
        }

        else if (ext == ".dcm")
        {
          var task = Application.Current.Dispatcher.BeginInvoke(new Action(() =>
          {
            LoadDcmVolume(Properties.Settings.Default.VolumeFile);
          }));
          //UpdateImage();
          //LogInfo("Volume Loaded.", 5000);
        }
      }, (x) =>
      {
        return File.Exists(Properties.Settings.Default.VolumeFile);
      });

      ExportImage = new RelayCommand((x) =>
      {
        _engine.SaveToImage();
      });
    }

    #endregion

    #region Properties

    public ImageSource ImageSrc
    {
      get { return _imageSrc; }
      set
      {
        if (value == _imageSrc)
          return;

        _imageSrc = value;
        base.OnPropertyChanged("ImageSrc");
      }
    }

    public string LogMessage
    {
      get { return _message; }
      set
      {
        if (_message != value)
        {
          _message = value;
          OnPropertyChanged("LogMessage");
        }
      }
    }

    public int FrameRate
    {
      get { return _frameRate; }
      set
      {
        if (value == _frameRate)
          return;

        _frameRate = value;

        base.OnPropertyChanged("FrameRate");
      }
    }

    public mivtmanaged.Application Engine
    {
      get {return _engine;}
    }

    public string ClassificationMode
    {
      get { return _classificationMode; }
      set
      {
        if (_classificationMode != value)
        {
          _classificationMode = value;

          _engine.SetClassificationMode(_classificationMode);
          UpdateImage();

          OnPropertyChanged("ClassificationMode");
        }
      }
    }

    public int ProtocalType
    {
      get { return _protocalType; }
      set
      {
        if (_protocalType != value)
        {
          _protocalType = value;

          _engine.SetTransfunc(ProtocalList[_protocalType].Name);
          UpdateImage();

          OnPropertyChanged("ProtocalType");
        }
      }
    }

    public List<ProtocalData> ProtocalList { get { return _protocalList; } }

    public List<string> ClassificationModeList { get { return _classificationModeList; } }

    public float Shiness
    {
      get { return _shiness; }
      set
      {
        if(_shiness!= value)
        {
          _shiness = value;

          _engine.SetMaterialShininess(_shiness);
          UpdateImage();

          OnPropertyChanged("Shiness");
        }
      }
    }

    public string AmbientColor
    {
      get { return _ambientColor; }
      set
      {
        if(_ambientColor != value)
        {
          _ambientColor = value;

          float[] arr = MyColorConverter.StringToArray(value);
          _engine.SetLightAmbient(arr);
          UpdateImage();

          OnPropertyChanged("AmbientColor");
        }
      }
    }

    public string DiffuseColor
    {
      get { return _diffuseColor; }
      set
      {
        if (_diffuseColor != value)
        {
          _diffuseColor = value;

          float[] arr = MyColorConverter.StringToArray(value);
          _engine.SetLightDiffuse(arr);
          UpdateImage();

          OnPropertyChanged("DiffuseColor");
        }
      }
    }

    public string SpecularColor
    {
      get { return _specularColor; }
      set
      {
        if (_specularColor != value)
        {
          _specularColor = value;

          float[] arr = MyColorConverter.StringToArray(value);
          _engine.SetLightSpecular(arr);
          UpdateImage();

          OnPropertyChanged("SpecularColor");
        }
      }
    }

    public List<string> BgColorModeList { get { return _bgColorModeList; } }

    public string BgColorMode
    {
      get { return _bgColorMode; }
      set
      {
        if (_bgColorMode != value)
        {
          _bgColorMode = value;

          _engine.SetBgColorMode(_bgColorMode);
          UpdateImage();

          OnPropertyChanged("BgColorMode");
        }
      }
    }

    public string FirstBgColor
    {
      get { return _firstBgColor; }
      set
      {
        if (_firstBgColor != value)
        {
          _firstBgColor = value;

          float[] arr = MyColorConverter.StringToArray(value);
          _engine.SetFirstBgColor(arr);
          UpdateImage();

          OnPropertyChanged("FirstBgColor");
        }
      }
    }

    public string SecondBgColor
    {
      get { return _secondBgColor; }
      set
      {
        if (_secondBgColor != value)
        {
          _secondBgColor = value;

          float[] arr = MyColorConverter.StringToArray(value);
          _engine.SetSecondBgColor(arr);
          UpdateImage();

          OnPropertyChanged("SecondBgColor");
        }
      }
    }

    public int ExportImageWidth
    {
      get { return _exportImageWidth; }
      set
      {
        if(_exportImageWidth != value)
        {
          _exportImageWidth = value;

          OnPropertyChanged("ExportImageWidth");
        }
      }
    }

    public int ExportImageHeight
    {
      get { return _exportImageHeight; }
      set
      {
        if (_exportImageHeight != value)
        {
          _exportImageHeight = value;

          OnPropertyChanged("ExportImageHeight");
        }
      }
    }

    #endregion

    #region Commands

    public ICommand ImageSizeChanged { get; private set; }

    public ICommand OpenSettingWindow { get; private set; }

    public ICommand LoadVolume { get; private set; }

    public ICommand OpenTransfuncWindow { get; private set; }

    public ICommand OpenLightWindow { get; private set; }

    public ICommand OpenExportWindow { get; private set; }

    public ICommand ExportImage { get; private set; }

    #endregion

    #region Public Methods

    private void UpdateImage(bool downsampling = false)
    {
      Stopwatch sw = new Stopwatch();
      sw.Start();

      if (_imageBuffer == null) return;
      _engine.GetPixels(_imageBuffer, downsampling);

      // copy image array into bitmap
      int stride = (_imageBitmap.PixelWidth * _imageBitmap.Format.BitsPerPixel + 7) / 8;
      _imageBitmap.WritePixels(new Int32Rect(0, 0, _imageBitmap.PixelWidth, 
        _imageBitmap.PixelHeight), _imageBuffer, stride, 0);
      ImageSrc = _imageBitmap;

      sw.Stop();
      FrameRate = (int)(1000 / Math.Max(sw.ElapsedMilliseconds, 1));
    }

    public void ImageMouseDown(object sender, MouseButtonEventArgs e)
    {
      _lastMousePosition = e.GetPosition(sender as Image);
      _mouseTracking = true;
    }

    public void ImageMouseUp(object sender, MouseButtonEventArgs e)
    {
      _mouseTracking = false;
      UpdateImage();
    }

    public void ImageMouseMove(object sender, MouseEventArgs e)
    {
      if(_mouseTracking)
      {
        Point mousePos = e.GetPosition(sender as Image);

        if(e.LeftButton == MouseButtonState.Pressed)
        {
          _engine.Rotate((int)mousePos.X, (int)mousePos.Y, 
            (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          UpdateImage(true);
        }
        else if(e.RightButton == MouseButtonState.Pressed)
        {
          _engine.Zoom((int)mousePos.X, (int)mousePos.Y, 
            (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          UpdateImage(true);
        }
        else if(e.MiddleButton == MouseButtonState.Pressed)
        {
          _engine.Pan((int)mousePos.X, (int)mousePos.Y, 
            (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          UpdateImage(true);
        }

        _lastMousePosition = mousePos;
      }
    }

    /// <summary>
    /// Set LogMsg properties which will in turn auto reset statusbar
    /// </summary>
    /// <param name="msg">message</param>
    /// <param name="delay">delay time of message rollback</param>
    private void LogInfo(string msg, int delay = 0)
    {
      _messageBackup = LogMessage;
      LogMessage = msg;

      // set delay timer to roll back message.
      if (delay > 0)
      {
        System.Threading.Tasks.Task.Factory.StartNew(() =>
        {
          System.Threading.Thread.Sleep(delay);
          LogInfo(_messageBackup);
        });
      }
    }

    private void LogInfo(string msg)
    {
      _messageBackup = LogMessage;
      LogMessage = msg;
    }


    private void LoadRawVolume(string rawFileName)
    {
      var infoFileName = System.IO.Path.ChangeExtension(rawFileName, ".info");
      // get volume information.
      int[] size = new int[3];
      float[] spacing = new float[3];
      string format;
      float[] interceptAndSlope = new float[2];
      float[] windowing = new float[2];
      try
      {
        StreamReader sr = new StreamReader(infoFileName);
        var tokens1 = sr.ReadLine().Split(',');
        var tokens2 = sr.ReadLine().Split(',');
        var tokens3 = sr.ReadLine().Split(',');
        var tokens4 = sr.ReadLine().Split(',');
        var tokens5 = sr.ReadLine().Split(',');

        for (int i = 0; i < 3; ++i)
        {
          size[i] = Int16.Parse(tokens1[i]);
          spacing[i] = (float)System.Double.Parse(tokens2[i]);
        }
        format = tokens3[0];
        for (int i = 0; i < 2; ++i)
        {
          interceptAndSlope[i] = (float)System.Double.Parse(tokens4[i]);
          windowing[i] = (float)System.Double.Parse(tokens5[i]);
        }
      }
      catch (FileNotFoundException)
      {
        LogInfo("Can not find \"" + infoFileName + "\"!");
        return;
      }
      catch (Exception)
      {
        LogInfo("Invalid info file format of \"" + infoFileName + "\"!");
        return;
      }
      _engine.LoadVolume(rawFileName, format, size, spacing, interceptAndSlope[0],
        interceptAndSlope[1], windowing[0], windowing[1]);
    }

    private void LoadDcmVolume(string fileName)
    {
      var thread = new Thread(() =>
      {
        _engine.LoadVolume(fileName, LogInfo);
      });
      thread.Start();
      //thread.Join();
    }

    #endregion

    public class ProtocalData
    {
      public string Photo { get; set; }
      public string Name { get; set; }
    }
  }
}
