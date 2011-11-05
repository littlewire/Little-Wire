namespace tinyPlot
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.myPlot = new ZedGraph.ZedGraphControl();
            this.myTimer = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // myPlot
            // 
            this.myPlot.Font = new System.Drawing.Font("Tahoma", 6F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.myPlot.Location = new System.Drawing.Point(12, 15);
            this.myPlot.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.myPlot.Name = "myPlot";
            this.myPlot.ScrollGrace = 0D;
            this.myPlot.ScrollMaxX = 0D;
            this.myPlot.ScrollMaxY = 0D;
            this.myPlot.ScrollMaxY2 = 0D;
            this.myPlot.ScrollMinX = 0D;
            this.myPlot.ScrollMinY = 0D;
            this.myPlot.ScrollMinY2 = 0D;
            this.myPlot.Size = new System.Drawing.Size(1108, 460);
            this.myPlot.TabIndex = 0;
            // 
            // myTimer
            // 
            this.myTimer.Interval = 1;
            this.myTimer.Tick += new System.EventHandler(this.myTimer_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1132, 488);
            this.Controls.Add(this.myPlot);
            this.Font = new System.Drawing.Font("Trebuchet MS", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.Text = "Analog Voltage - Real Time Plot";
            this.ResumeLayout(false);

        }

        #endregion

        private ZedGraph.ZedGraphControl myPlot;
        private System.Windows.Forms.Timer myTimer;
    }
}

