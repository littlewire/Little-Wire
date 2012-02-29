using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Threading;
using System.Text;
using System.Diagnostics;
using System.Windows.Forms;
using littleWireLib;
using ZedGraph;

namespace tinyPlot
{
    public partial class Form1 : Form
    {
        littleWire myDevice=new littleWire();
        PointPairList myPoints=new PointPairList();
        LineItem myLine;
        GraphPane myPane;
        YAxis myAxis = new YAxis("myAxis");
        int plotWidth = 100;
        int i = 0;
        public Form1()
        {
            InitializeComponent();
            myPane = myPlot.GraphPane;
        
            int status = myDevice.connect();
            if (status == 0)
            {
                MessageBox.Show("Device couldn't be found!");
                Environment.Exit(0);
            }
            
            myPane.XAxis.Scale.Min = 0;
            myPane.XAxis.Scale.Max = plotWidth;
            myPane.YAxis.Scale.Min = 0;
            myPane.YAxis.Scale.Max = 5;

            myDevice.pinMode(2, 1);

            myLine = myPane.AddCurve("Analog Voltage", myPoints, Color.Blue, SymbolType.None);
            myDevice.initPwm();
  
            myLine.Line.Width = 1;

            myPlot.Invalidate();
            myPlot.AxisChange();
            
            myTimer.Start();
        }

        private void myTimer_Tick(object sender, EventArgs e)
        {
            double temp = ((myDevice.analogRead(1) * 5.0) / 1024.0);
            myPoints.Add(i++,temp);
            if (i > plotWidth)
            {
                myPane.XAxis.Scale.Min = i - plotWidth;
                myPane.XAxis.Scale.Max = i;
            }
            myPlot.Invalidate();
            myPlot.AxisChange();
        }
    }
}
