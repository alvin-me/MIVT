using System;
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
        PixelFormat pf = PixelFormats.Bgr24;
        int stride = (width * pf.BitsPerPixel + 7) / 8;
        _imageBitmap = new WriteableBitmap(width, height, 96, 96, pf, null);

        _engine.Resize(width, height);

        UpdateImage();
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

    #endregion

    #region Commands

    public ICommand ImageSizeChanged { get; private set; }

    #endregion

    #region Public Methods

    private void UpdateImage()
    {
      _engine.GetPixels(_imageBuffer);

      // copy image array into bitmap
      int stride = (_imageBitmap.PixelWidth * _imageBitmap.Format.BitsPerPixel + 7) / 8;
      _imageBitmap.WritePixels(new Int32Rect(0, 0, _imageBitmap.PixelWidth, 
        _imageBitmap.PixelHeight), _imageBuffer, stride, 0);
      ImageSrc = _imageBitmap;
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

    #endregion
  }
}
