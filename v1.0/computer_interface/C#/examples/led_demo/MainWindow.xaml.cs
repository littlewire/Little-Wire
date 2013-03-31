/*
 * LittleWire LED Demo 
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
using System.Windows.Shapes;
using littleWireLib;

namespace led_demo
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class Main : Window
    {
        littleWire lw = new littleWire();

        struct led
        {
            public byte pin;
            public bool state;
            public Image icon;

            public led(byte _pin, bool _state, Image _icon)
            {
                pin = _pin;
                state = _state;
                icon = _icon;
            }
        }
        led led1, led2, led3, led4;

        // Icons from: http://findicons.com/pack/2428/woocons
        BitmapImage bulb_on = new BitmapImage(new Uri(@"/led_demo;component/light_bulb_on.png", UriKind.Relative));
        BitmapImage bulb_off = new BitmapImage(new Uri(@"/led_demo;component/light_bulb_off.png", UriKind.Relative));

        public Main()
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

            // Initiliase LEDs and their states
            led1 = new led(littleWire.PIN1, false, bulb1);
            led2 = new led(littleWire.PIN2, false, bulb2);
            led3 = new led(littleWire.PIN3, false, bulb3);
            led4 = new led(littleWire.PIN4, false, bulb4);

            // Set up pin directions on the device
            lw.pinMode(led1.pin, littleWire.OUTPUT);
            lw.pinMode(led2.pin, littleWire.OUTPUT);
            lw.pinMode(led3.pin, littleWire.OUTPUT);
            lw.pinMode(led4.pin, littleWire.OUTPUT);

            // Clear pins, just in case
            lw.digitalWrite(led1.pin, littleWire.LOW);
            lw.digitalWrite(led2.pin, littleWire.LOW);
            lw.digitalWrite(led3.pin, littleWire.LOW);
            lw.digitalWrite(led4.pin, littleWire.LOW);

        }
        #endregion

        #region Button event listeners
        private void button_pin1_Click(object sender, RoutedEventArgs e)
        {
            toggle_led(ref led1);
        }

        private void button_pin2_Click(object sender, RoutedEventArgs e)
        {
            toggle_led(ref led2);
        }

        private void button_pin3_Click(object sender, RoutedEventArgs e)
        {
            toggle_led(ref led3);
        }

        private void button_pin4_Click(object sender, RoutedEventArgs e)
        {
            toggle_led(ref led4);
        }
        #endregion

        #region Toggle LED function
        private void toggle_led(ref led led)
        {
            // Toggle the bulb and the pin state
            if (led.state)
            {
                lw.digitalWrite(led.pin, littleWire.LOW);
                led.icon.Source = bulb_off;
                led.state = false;
            }
            else
            {
                lw.digitalWrite(led.pin, littleWire.HIGH);
                led.icon.Source = bulb_on;
                led.state = true;
            }
        }
        #endregion
    }
}
