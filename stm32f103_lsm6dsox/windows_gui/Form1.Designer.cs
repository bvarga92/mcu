namespace lsm6dsox_gui
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
            this.pictureBoxAcc = new System.Windows.Forms.PictureBox();
            this.pictureBoxGyr = new System.Windows.Forms.PictureBox();
            this.labelTemp = new System.Windows.Forms.Label();
            this.labelAcc = new System.Windows.Forms.Label();
            this.labelGyr = new System.Windows.Forms.Label();
            this.chbLog = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxAcc)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGyr)).BeginInit();
            this.SuspendLayout();
            // 
            // cbPorts
            // 
            this.cbPorts.FormattingEnabled = true;
            this.cbPorts.Location = new System.Drawing.Point(12, 8);
            this.cbPorts.Name = "cbPorts";
            this.cbPorts.Size = new System.Drawing.Size(90, 21);
            this.cbPorts.TabIndex = 0;
            // 
            // btnRescan
            // 
            this.btnRescan.Location = new System.Drawing.Point(117, 8);
            this.btnRescan.Name = "btnRescan";
            this.btnRescan.Size = new System.Drawing.Size(90, 23);
            this.btnRescan.TabIndex = 1;
            this.btnRescan.Text = "Rescan";
            this.btnRescan.UseVisualStyleBackColor = true;
            this.btnRescan.Click += new System.EventHandler(this.btnRescan_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(222, 8);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(90, 23);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // pictureBoxAcc
            // 
            this.pictureBoxAcc.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBoxAcc.Location = new System.Drawing.Point(12, 88);
            this.pictureBoxAcc.Name = "pictureBoxAcc";
            this.pictureBoxAcc.Size = new System.Drawing.Size(300, 130);
            this.pictureBoxAcc.TabIndex = 3;
            this.pictureBoxAcc.TabStop = false;
            // 
            // pictureBoxGyr
            // 
            this.pictureBoxGyr.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pictureBoxGyr.Location = new System.Drawing.Point(12, 243);
            this.pictureBoxGyr.Name = "pictureBoxGyr";
            this.pictureBoxGyr.Size = new System.Drawing.Size(300, 130);
            this.pictureBoxGyr.TabIndex = 5;
            this.pictureBoxGyr.TabStop = false;
            // 
            // labelTemp
            // 
            this.labelTemp.AutoSize = true;
            this.labelTemp.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelTemp.Location = new System.Drawing.Point(13, 40);
            this.labelTemp.Name = "labelTemp";
            this.labelTemp.Size = new System.Drawing.Size(106, 17);
            this.labelTemp.TabIndex = 6;
            this.labelTemp.Text = "Temperature:";
            // 
            // labelAcc
            // 
            this.labelAcc.AutoSize = true;
            this.labelAcc.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelAcc.Location = new System.Drawing.Point(78, 68);
            this.labelAcc.Name = "labelAcc";
            this.labelAcc.Size = new System.Drawing.Size(166, 17);
            this.labelAcc.TabIndex = 7;
            this.labelAcc.Text = "Accelerometer Output";
            // 
            // labelGyr
            // 
            this.labelGyr.AutoSize = true;
            this.labelGyr.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.labelGyr.Location = new System.Drawing.Point(91, 223);
            this.labelGyr.Name = "labelGyr";
            this.labelGyr.Size = new System.Drawing.Size(140, 17);
            this.labelGyr.TabIndex = 8;
            this.labelGyr.Text = "Gyroscope Output";
            // 
            // chbLog
            // 
            this.chbLog.AutoSize = true;
            this.chbLog.Checked = true;
            this.chbLog.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chbLog.Location = new System.Drawing.Point(242, 41);
            this.chbLog.Name = "chbLog";
            this.chbLog.Size = new System.Drawing.Size(70, 17);
            this.chbLog.TabIndex = 9;
            this.chbLog.Text = "Log Data";
            this.chbLog.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(324, 383);
            this.Controls.Add(this.chbLog);
            this.Controls.Add(this.labelGyr);
            this.Controls.Add(this.labelAcc);
            this.Controls.Add(this.labelTemp);
            this.Controls.Add(this.pictureBoxGyr);
            this.Controls.Add(this.pictureBoxAcc);
            this.Controls.Add(this.btnRescan);
            this.Controls.Add(this.cbPorts);
            this.Controls.Add(this.btnConnect);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "LSM6DSOX Logger";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxAcc)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxGyr)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbPorts;
        private System.Windows.Forms.Button btnRescan;
        private System.Windows.Forms.PictureBox pictureBoxAcc;
        private System.Windows.Forms.PictureBox pictureBoxGyr;
        private System.Windows.Forms.Label labelTemp;
        private System.Windows.Forms.Label labelAcc;
        private System.Windows.Forms.Label labelGyr;
        private System.Windows.Forms.CheckBox chbLog;
    }
}

