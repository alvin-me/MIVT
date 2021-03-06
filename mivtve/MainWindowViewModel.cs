﻿using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
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
    private ClippingWindow          _clippingWindow = null;
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
    private float                   _clipLeft;
    private float                   _clipRight;
    private float                   _clipFront;
    private float                   _clipBack;
    private float                   _clipTop;
    private float                   _clipBottom;
    private int                     _clipRangeX;
    private int                     _clipRangeY;
    private int                     _clipRangeZ;
    private bool                    _volumeLoaded;
    private ObservableCollection<Point> _polygonPoints;
    private bool                    _onSculpt;
    private float                   _windowWidth;
    private float                   _windowCenter;

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

      ClipRangeX = 1;
      ClipRangeY = 1;
      ClipRangeZ = 1;

      _volumeLoaded = false;

      _polygonPoints = new ObservableCollection<Point>();

      _onSculpt = false;

      // initialize commands

      ImageSizeChanged = new RelayCommand((x) =>
      {
        SizeChangedEventArgs args = (SizeChangedEventArgs)((Tuple<object, EventArgs>)x).Item2;
        int width = (int)args.NewSize.Width / 4 * 4;
        int height = (int)args.NewSize.Height;

        _imageBuffer = new byte[width * height * 4];

        // create bitmap for control binding
        PixelFormat pf = PixelFormats.Bgra32;
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

      OpenClippingWindow = new RelayCommand((x) =>
      {
        if(_clippingWindow == null)
        {
          _clippingWindow = new ClippingWindow();

          int[] range = new int[3];
          _engine.getClipMaximum(range);
          ClipRangeX = range[0];
          ClipRangeY = range[1];
          ClipRangeZ = range[2];

          ClipRight = _engine.GetClipRight();
          ClipLeft = _engine.GetClipLeft();
          ClipBottom = _engine.GetClipBottom();
          ClipTop = _engine.GetClipTop();
          ClipFront = _engine.GetClipFront();
          ClipBack = _engine.GetClipBack();

          _clippingWindow.DataContext = this;
        }
        _clippingWindow.Show();
      }, 
      (x) =>
      {
        return _volumeLoaded;
      });

      LoadVolume = new RelayCommand((x) =>
      {
        string ext = Path.GetExtension(Properties.Settings.Default.VolumeFile);
        if(ext == ".img" || ext == ".raw") {
          LoadRawVolume(Properties.Settings.Default.VolumeFile);
          UpdateImage();
          LogInfo("Volume Loaded.", 5000);
          _volumeLoaded = true;
        }
        else if (ext == ".dcm")
        {
          var task = Application.Current.Dispatcher.BeginInvoke(new Action(() =>
          {
            LoadDcmVolume(Properties.Settings.Default.VolumeFile);
          }));
          //UpdateImage();
          //LogInfo("Volume Loaded.", 5000);
          _volumeLoaded = true;
        }

        int[] range = new int[3];
        _engine.getClipMaximum(range);
        ClipRangeX = range[0];
        ClipRangeY = range[1];
        ClipRangeZ = range[2];

        float[] domain = new float[2];
        _engine.getWindowingDomain(domain);
        WindowWidth = domain[1] - domain[0];
        WindowCenter = domain[0] + (domain[1] - domain[0]) * 0.5f;

      }, (x) =>
      {
        return File.Exists(Properties.Settings.Default.VolumeFile);
      });

      ExportImage = new RelayCommand((x) =>
      {
        _engine.SaveToImage(ExportImageWidth, ExportImageHeight);
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

    public float ClipLeft
    {
      get { return _clipLeft; }
      set
      {
        if (_clipLeft != value)
        {
          _clipLeft = value;

          _engine.ChangeClipLeft(value);
          UpdateImage();

          OnPropertyChanged("ClipLeft");
        }
      }
    }

    public float ClipRight
    {
      get { return _clipRight; }
      set
      {
        if (_clipRight != value)
        {
          _clipRight = value;

          _engine.ChangeClipRight(value);
          UpdateImage();

          OnPropertyChanged("ClipRight");
        }
      }
    }

    public float ClipFront
    {
      get { return _clipFront; }
      set
      {
        if (_clipFront != value)
        {
          _clipFront = value;

          _engine.ChangeClipFront(value);
          UpdateImage();

          OnPropertyChanged("ClipFront");
        }
      }
    }

    public float ClipBack
    {
      get { return _clipBack; }
      set
      {
        if (_clipBack != value)
        {
          _clipBack = value;

          _engine.ChangeClipBack(value);
          UpdateImage();

          OnPropertyChanged("ClipBack");
        }
      }
    }

    public float ClipTop
    {
      get { return _clipTop; }
      set
      {
        if (_clipTop != value)
        {
          _clipTop = value;

          _engine.ChangeClipTop(value);
          UpdateImage();

          OnPropertyChanged("ClipTop");
        }
      }
    }

    public float ClipBottom
    {
      get { return _clipBottom; }
      set
      {
        if (_clipBottom != value)
        {
          _clipBottom = value;

          _engine.ChangeClipBottom(value);
          UpdateImage();

          OnPropertyChanged("ClipBottom");
        }
      }
    }

    public int ClipRangeX
    {
      get { return _clipRangeX; }
      set
      {
        if (_clipRangeX != value)
        {
          _clipRangeX = value;

          OnPropertyChanged("ClipRangeX");
        }
      }
    }

    public int ClipRangeY
    {
      get { return _clipRangeY; }
      set
      {
        if (_clipRangeY != value)
        {
          _clipRangeY = value;

          OnPropertyChanged("ClipRangeY");
        }
      }
    }

    public int ClipRangeZ
    {
      get { return _clipRangeZ; }
      set
      {
        if (_clipRangeZ != value)
        {
          _clipRangeZ = value;

          OnPropertyChanged("ClipRangeZ");
        }
      }
    }

    public ObservableCollection<Point> PolygonPoints
    {
      get { return _polygonPoints; }
      set
      {
        if (_polygonPoints != value)
        {
          _polygonPoints = value;

          OnPropertyChanged("PolygonPoints");
        }
      }
    }

    public bool OnSculpt
    {
      get { return _onSculpt; }
      set
      {
        if (_onSculpt != value)
        {
          _onSculpt = value;

          OnPropertyChanged("OnSculpt");
        }
      }
    }

    public float WindowWidth
    {
      get { return _windowWidth; }
      set
      {
        if (_windowWidth != value)
        {
          _windowWidth = value;

          OnPropertyChanged("WindowWidth");
        }
      }
    }

    public float WindowCenter
    {
      get { return _windowCenter; }
      set
      {
        if (_windowCenter != value)
        {
          _windowCenter = value;

          OnPropertyChanged("WindowCenter");
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

    public ICommand OpenClippingWindow { get; private set; }

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
      _lastMousePosition = e.GetPosition(sender as Grid);
      _mouseTracking = true;

      if (OnSculpt)
      {
        PolygonPoints.Clear();
        PolygonPoints.Add(new Point(Math.Round(_lastMousePosition.X), Math.Round(_lastMousePosition.Y)));
        OnPropertyChanged("PolygonPoints");
      }
    }

    public void ImageMouseUp(object sender, MouseButtonEventArgs e)
    {
      if (OnSculpt)
      {
        OnSculpt = false;

        float []arr = new float[PolygonPoints.Count * 2];
        for(int i = 0; i < PolygonPoints.Count; ++i) 
        {
          arr[i*2] = (float)PolygonPoints[i].X;
          arr[i * 2 + 1] = (float)PolygonPoints[i].Y;
        }
        _engine.DoSculpt(arr);

        PolygonPoints.Clear();
        OnPropertyChanged("PolygonPoints");
      }

      _mouseTracking = false;
      UpdateImage();
    }

    public void ImageMouseMove(object sender, MouseEventArgs e)
    {
      if(_mouseTracking)
      {
        Point mousePos = e.GetPosition(sender as Grid);

        if(e.LeftButton == MouseButtonState.Pressed)
        {
          if (OnSculpt)
          {
            Point p = new Point(Math.Round(mousePos.X), Math.Round(mousePos.Y));
            if(p.X != PolygonPoints[PolygonPoints.Count-1].X ||
              p.Y != PolygonPoints[PolygonPoints.Count - 1].Y)
            {
              PolygonPoints.Add(p);
              OnPropertyChanged("PolygonPoints");
            }

            return;
          }

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
          if (Keyboard.IsKeyDown(Key.LeftCtrl) || Keyboard.IsKeyDown(Key.RightCtrl))
          {
            float dx = (float)(mousePos.X - _lastMousePosition.X);
            float dy = (float)(mousePos.Y - _lastMousePosition.Y);
            WindowWidth += dx;
            WindowCenter += dy;
            float[] domain = new float[2];
            domain[0] = WindowCenter - WindowWidth * 0.5f;
            domain[1] = WindowCenter + WindowWidth * 0.5f;
            _engine.setWindowingDomain(domain);
          }
          else
          {
            _engine.Pan((int)mousePos.X, (int)mousePos.Y,
              (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          }

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
