/*
 * LittleWire PWM Demo 
 * Omer Kilic, June 2012
*/

using System;
using System.Collections.Generic;
using System.Linq;
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
using littleWireLib;

namespace pwm_demo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        littleWire lw = new littleWire();

        public MainWindow()
        {
            InitializeComponent();
        }

        #region Initialisation Code
        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // Can we actually connect to the unit?
            int status = lw.connect();
            if (status == 0)
            {
                MessageBox.Show("Device couldn't be found!");
                Environment.Exit(1);
            }

            // Initialise PWM
            lw.initPwm();

            // Reset PWM
            lw.updatePwmPrescale(1);
            lw.updatePwmCompare(0, 0);

            // Add Event Listener for the Prescaler Selection Combobox
            // (this has to happen after lw.connect())
            prescaler.SelectionChanged += prescaler_SelectionChanged;
        }
        #endregion

        #region Event listeners
        private void slider_a_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            // Update channels
            update_pwm();

            // Update reading
            label_a.Content = "Channel A (Pin 4)    Current: " + slider_a.Value.ToString();

        }

        private void slider_b_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            // Update channels
            update_pwm();

            // Update reading
            label_b.Content = "Channel B (Pin 1)    Current: " + slider_b.Value.ToString();
        }

        private void prescaler_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            switch (prescaler.SelectedIndex)
            {
                case 0:
                    lw.updatePwmPrescale(1);
                    break;
                case 1:
                    lw.updatePwmPrescale(8);
                    break;
                case 2:
                    lw.updatePwmPrescale(64);
                    break;
                case 3:
                    lw.updatePwmPrescale(256);
                    break;
                case 4:
                    lw.updatePwmPrescale(1024);
                    break;
            }
        }
        #endregion

        #region Update PWM function
        public void update_pwm()
        {
            byte value_a = System.Convert.ToByte(slider_a.Value);
            byte value_b = System.Convert.ToByte(slider_b.Value);

            lw.updatePwmCompare(value_a, value_b);
        }
        #endregion
    }
}
