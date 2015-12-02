using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace mivtve
{
  /// <summary>
  /// Interaction logic for ClippingWindow.xaml
  /// </summary>
  public partial class ClippingWindow : Window
  {
    public ClippingWindow()
    {
      InitializeComponent();
    }

    protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
    {
      //base.OnClosing(e);
      this.Visibility = Visibility.Hidden;
      e.Cancel = true;
    }
  }
}
