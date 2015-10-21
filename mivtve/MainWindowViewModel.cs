using System;
using System.Diagnostics;
using System.IO;
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

    #endregion

    #region Constructors

    public MainWindowViewModel()
    {
      _engine = new mivtmanaged.Application();

      _mouseTracking = false;

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
      });

      OpenSettingWindow = new RelayCommand((x) =>
      {
        SettingWindow window = new SettingWindow();
        if (window.ShowDialog() == true)
        {

        }
      });

      LoadVolume = new RelayCommand((x) =>
      {
        var rawFileName = Properties.Settings.Default.VolumeFile;
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
        UpdateImage();
        LogInfo("Volume Loaded.", 5000);
      }, (x) =>
      {
        return File.Exists(Properties.Settings.Default.VolumeFile);
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

    #endregion

    #region Commands

    public ICommand ImageSizeChanged { get; private set; }

    public ICommand OpenSettingWindow { get; private set; }

    public ICommand LoadVolume { get; private set; }

    #endregion

    #region Public Methods

    private void UpdateImage()
    {
      Stopwatch sw = new Stopwatch();
      sw.Start();

      _engine.GetPixels(_imageBuffer);

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
          UpdateImage();
        }
        else if(e.RightButton == MouseButtonState.Pressed)
        {
          _engine.Zoom((int)mousePos.X, (int)mousePos.Y, 
            (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          UpdateImage();
        }
        else if(e.MiddleButton == MouseButtonState.Pressed)
        {
          _engine.Pan((int)mousePos.X, (int)mousePos.Y, 
            (int)_lastMousePosition.X, (int)_lastMousePosition.Y);
          UpdateImage();
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

    #endregion
  }
}
