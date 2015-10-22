using System;
using System.Collections.Generic;
using System.IO;
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

namespace mivtve
{
  /// <summary>
  /// Interaction logic for TransfuncWindow.xaml
  /// </summary>
  public partial class TransfuncWindow : Window
  {
    public TransfuncWindow()
    {
      InitializeComponent();
      InitializeControls();
    }

    public void InitializeControls()
    {
      this.cmbClassficationMode.Items.Add("transfer-function");
      this.cmbClassficationMode.Items.Add("pre-integrated");

      //string basePath = System.Reflection.Assembly.GetExecutingAssembly().Location;
      string basePath = System.AppDomain.CurrentDomain.BaseDirectory;
      basePath += "..\\..\\resource\\transfuncs\\";

      List<String> fileList = new List<string>();
      foreach(var f in Directory.GetFiles(basePath))
      {
        fileList.Add(Path.GetFileNameWithoutExtension(f));
      }

      this.cmbProtocalType.ItemsSource = fileList;
    }
  }
}
