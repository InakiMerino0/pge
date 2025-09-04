using System.IO;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ejercicio1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private double convertTemp(double celsiusDegrees)
        {
            double farenheits = (celsiusDegrees * 1.8) + 32;
            return farenheits;
        }

        private void updateLabel(double farenheits)
        {
            convertedDataLabel.Content = farenheits.ToString();
        }

        private void TextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {   
                double farenheits = convertTemp(float.Parse(textBoxCelsius.Text));
                updateLabel(farenheits);
            } catch (FormatException)
            {
                convertedDataLabel.Content = "Ingrese un valor numerico";
            }
        }
    }
}