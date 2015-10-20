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
  /// Interaction logic for SettingWindow.xaml
  /// </summary>
  public partial class SettingWindow : Window
  {
    public SettingWindow()
    {
      InitializeComponent();
      InitializeSettingFromConfigFile();

      this.btnSetVolumeFile.Click += new RoutedEventHandler(setVolumeFile);
    }

    protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
    {
      if (Properties.Settings.Default.VolumeFile != this.txtVolumeFile.Text)
      {
        saveSetting();
      }
      base.DialogResult = true;
      base.OnClosing(e);
    }

    private void InitializeSettingFromConfigFile()
    {
      this.txtVolumeFile.Text = Properties.Settings.Default.VolumeFile;
    }

    private void saveSetting()
    {
      Properties.Settings.Default.VolumeFile = this.txtVolumeFile.Text;
      Properties.Settings.Default.Save();
    }

    private void setVolumeFile(object sender, RoutedEventArgs e)
    {
      // select folder
      //var fbd = new System.Windows.Forms.FolderBrowserDialog();
      //fbd.RootFolder = Environment.SpecialFolder.Desktop;
      //fbd.Description = "Please Select Volume Files:";
      //if (fbd.ShowDialog() == System.Windows.Forms.DialogResult.OK)
      //{
      //  this.txtVolumeFile.Text = fbd.SelectedPath;
      //}

      // select file
      var fd = new Microsoft.Win32.OpenFileDialog();
      fd.InitialDirectory = Environment.SpecialFolder.Desktop.ToString();
      fd.RestoreDirectory = true;
      fd.Title = "Please Select Volume Files:";
      fd.Filter = "RAW Files (*.raw, *.img)|*.raw; *.img|All Files(*.*)|*.*";
      if(fd.ShowDialog() == true)
      {
        this.txtVolumeFile.Text = fd.FileName;
      }

    }
  }
}
