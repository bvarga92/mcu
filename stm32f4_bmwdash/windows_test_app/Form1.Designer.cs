namespace bmwSerial
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
            this.cbPorts = new System.Windows.Forms.ComboBox();
            this.btnRescan = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnSignalLeft = new System.Windows.Forms.Button();
            this.btnHighBeam = new System.Windows.Forms.Button();
            this.btnSignalRight = new System.Windows.Forms.Button();
            this.tbSpeed = new System.Windows.Forms.TrackBar();
            this.tbRPM = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.tbSpeed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbRPM)).BeginInit();
            this.SuspendLayout();
            // 
            // cbPorts
            // 
            this.cbPorts.FormattingEnabled = true;
            this.cbPorts.Location = new System.Drawing.Point(12, 10);
            this.cbPorts.Name = "cbPorts";
            this.cbPorts.Size = new System.Drawing.Size(75, 21);
            this.cbPorts.TabIndex = 0;
            // 
            // btnRescan
            // 
            this.btnRescan.Location = new System.Drawing.Point(103, 10);
            this.btnRescan.Name = "btnRescan";
            this.btnRescan.Size = new System.Drawing.Size(75, 23);
            this.btnRescan.TabIndex = 1;
            this.btnRescan.Text = "Rescan";
            this.btnRescan.UseVisualStyleBackColor = true;
            this.btnRescan.Click += new System.EventHandler(this.btnRescan_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(193, 10);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnSignalLeft
            // 
            this.btnSignalLeft.Location = new System.Drawing.Point(12, 51);
            this.btnSignalLeft.Name = "btnSignalLeft";
            this.btnSignalLeft.Size = new System.Drawing.Size(75, 23);
            this.btnSignalLeft.TabIndex = 3;
            this.btnSignalLeft.Text = "Signal Left";
            this.btnSignalLeft.UseVisualStyleBackColor = true;
            this.btnSignalLeft.Click += new System.EventHandler(this.btnSignalLeft_Click);
            // 
            // btnHighBeam
            // 
            this.btnHighBeam.Location = new System.Drawing.Point(103, 51);
            this.btnHighBeam.Name = "btnHighBeam";
            this.btnHighBeam.Size = new System.Drawing.Size(75, 23);
            this.btnHighBeam.TabIndex = 4;
            this.btnHighBeam.Text = "High Beam";
            this.btnHighBeam.UseVisualStyleBackColor = true;
            this.btnHighBeam.Click += new System.EventHandler(this.btnHighBeam_Click);
            // 
            // btnSignalRight
            // 
            this.btnSignalRight.Location = new System.Drawing.Point(193, 51);
            this.btnSignalRight.Name = "btnSignalRight";
            this.btnSignalRight.Size = new System.Drawing.Size(75, 23);
            this.btnSignalRight.TabIndex = 5;
            this.btnSignalRight.Text = "Signal Right";
            this.btnSignalRight.UseVisualStyleBackColor = true;
            this.btnSignalRight.Click += new System.EventHandler(this.btnSignalRight_Click);
            // 
            // tbSpeed
            // 
            this.tbSpeed.LargeChange = 10;
            this.tbSpeed.Location = new System.Drawing.Point(12, 94);
            this.tbSpeed.Maximum = 240;
            this.tbSpeed.Name = "tbSpeed";
            this.tbSpeed.Size = new System.Drawing.Size(256, 45);
            this.tbSpeed.SmallChange = 5;
            this.tbSpeed.TabIndex = 6;
            this.tbSpeed.Scroll += new System.EventHandler(this.tbSpeed_Scroll);
            // 
            // tbRPM
            // 
            this.tbRPM.LargeChange = 50;
            this.tbRPM.Location = new System.Drawing.Point(12, 146);
            this.tbRPM.Maximum = 7000;
            this.tbRPM.Name = "tbRPM";
            this.tbRPM.Size = new System.Drawing.Size(256, 45);
            this.tbRPM.SmallChange = 10;
            this.tbRPM.TabIndex = 7;
            this.tbRPM.Scroll += new System.EventHandler(this.tbRPM_Scroll);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(280, 206);
            this.Controls.Add(this.btnRescan);
            this.Controls.Add(this.tbRPM);
            this.Controls.Add(this.tbSpeed);
            this.Controls.Add(this.btnSignalRight);
            this.Controls.Add(this.btnHighBeam);
            this.Controls.Add(this.btnSignalLeft);
            this.Controls.Add(this.cbPorts);
            this.Controls.Add(this.btnConnect);
            this.Name = "Form1";
            this.Text = "BMW E46 USB";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.tbSpeed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tbRPM)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbPorts;
        private System.Windows.Forms.Button btnSignalLeft;
        private System.Windows.Forms.Button btnHighBeam;
        private System.Windows.Forms.Button btnSignalRight;
        private System.Windows.Forms.TrackBar tbSpeed;
        private System.Windows.Forms.TrackBar tbRPM;
        private System.Windows.Forms.Button btnRescan;
    }
}

