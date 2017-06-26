namespace usbio
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
            this.btnSend = new System.Windows.Forms.Button();
            this.btnRescan = new System.Windows.Forms.Button();
            this.cbPorts = new System.Windows.Forms.ComboBox();
            this.btnConnect = new System.Windows.Forms.Button();
            this.tbLine1 = new System.Windows.Forms.TextBox();
            this.tbLine2 = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.tb0 = new System.Windows.Forms.TrackBar();
            this.tb2 = new System.Windows.Forms.TrackBar();
            this.tb4 = new System.Windows.Forms.TrackBar();
            this.tb6 = new System.Windows.Forms.TrackBar();
            this.tb8 = new System.Windows.Forms.TrackBar();
            this.tb9 = new System.Windows.Forms.TrackBar();
            this.tb7 = new System.Windows.Forms.TrackBar();
            this.tb5 = new System.Windows.Forms.TrackBar();
            this.tb3 = new System.Windows.Forms.TrackBar();
            this.tb1 = new System.Windows.Forms.TrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.tb0)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb4)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb6)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb8)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb9)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb7)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb5)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb3)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb1)).BeginInit();
            this.SuspendLayout();
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(441, 12);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 48);
            this.btnSend.TabIndex = 6;
            this.btnSend.Text = "Send Text";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // btnRescan
            // 
            this.btnRescan.Location = new System.Drawing.Point(96, 25);
            this.btnRescan.Name = "btnRescan";
            this.btnRescan.Size = new System.Drawing.Size(75, 23);
            this.btnRescan.TabIndex = 2;
            this.btnRescan.Text = "Rescan";
            this.btnRescan.UseVisualStyleBackColor = true;
            this.btnRescan.Click += new System.EventHandler(this.btnRescan_Click);
            // 
            // cbPorts
            // 
            this.cbPorts.FormattingEnabled = true;
            this.cbPorts.Location = new System.Drawing.Point(24, 25);
            this.cbPorts.Name = "cbPorts";
            this.cbPorts.Size = new System.Drawing.Size(63, 21);
            this.cbPorts.TabIndex = 1;
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(179, 25);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 3;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // tbLine1
            // 
            this.tbLine1.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.tbLine1.Location = new System.Drawing.Point(293, 12);
            this.tbLine1.MaxLength = 16;
            this.tbLine1.Name = "tbLine1";
            this.tbLine1.Size = new System.Drawing.Size(129, 20);
            this.tbLine1.TabIndex = 4;
            this.tbLine1.Text = "Helló világ! :)";
            // 
            // tbLine2
            // 
            this.tbLine2.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.tbLine2.Location = new System.Drawing.Point(293, 40);
            this.tbLine2.MaxLength = 16;
            this.tbLine2.Name = "tbLine2";
            this.tbLine2.Size = new System.Drawing.Size(129, 20);
            this.tbLine2.TabIndex = 5;
            // 
            // timer1
            // 
            this.timer1.Interval = 20;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // tb0
            // 
            this.tb0.Enabled = false;
            this.tb0.LargeChange = 1;
            this.tb0.Location = new System.Drawing.Point(421, 73);
            this.tb0.Maximum = 255;
            this.tb0.Name = "tb0";
            this.tb0.Size = new System.Drawing.Size(95, 45);
            this.tb0.TabIndex = 18;
            this.tb0.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb2
            // 
            this.tb2.Enabled = false;
            this.tb2.LargeChange = 1;
            this.tb2.Location = new System.Drawing.Point(320, 73);
            this.tb2.Maximum = 255;
            this.tb2.Name = "tb2";
            this.tb2.Size = new System.Drawing.Size(95, 45);
            this.tb2.TabIndex = 19;
            this.tb2.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb4
            // 
            this.tb4.Enabled = false;
            this.tb4.LargeChange = 1;
            this.tb4.Location = new System.Drawing.Point(219, 73);
            this.tb4.Maximum = 255;
            this.tb4.Name = "tb4";
            this.tb4.Size = new System.Drawing.Size(95, 45);
            this.tb4.TabIndex = 20;
            this.tb4.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb6
            // 
            this.tb6.Enabled = false;
            this.tb6.LargeChange = 1;
            this.tb6.Location = new System.Drawing.Point(118, 73);
            this.tb6.Maximum = 255;
            this.tb6.Name = "tb6";
            this.tb6.Size = new System.Drawing.Size(95, 45);
            this.tb6.TabIndex = 21;
            this.tb6.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb8
            // 
            this.tb8.Enabled = false;
            this.tb8.LargeChange = 1;
            this.tb8.Location = new System.Drawing.Point(17, 73);
            this.tb8.Maximum = 255;
            this.tb8.Name = "tb8";
            this.tb8.Size = new System.Drawing.Size(95, 45);
            this.tb8.TabIndex = 22;
            this.tb8.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb9
            // 
            this.tb9.Enabled = false;
            this.tb9.LargeChange = 1;
            this.tb9.Location = new System.Drawing.Point(17, 104);
            this.tb9.Maximum = 255;
            this.tb9.Name = "tb9";
            this.tb9.Size = new System.Drawing.Size(95, 45);
            this.tb9.TabIndex = 27;
            this.tb9.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb7
            // 
            this.tb7.Enabled = false;
            this.tb7.LargeChange = 1;
            this.tb7.Location = new System.Drawing.Point(118, 104);
            this.tb7.Maximum = 255;
            this.tb7.Name = "tb7";
            this.tb7.Size = new System.Drawing.Size(95, 45);
            this.tb7.TabIndex = 26;
            this.tb7.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb5
            // 
            this.tb5.Enabled = false;
            this.tb5.LargeChange = 1;
            this.tb5.Location = new System.Drawing.Point(219, 104);
            this.tb5.Maximum = 255;
            this.tb5.Name = "tb5";
            this.tb5.Size = new System.Drawing.Size(95, 45);
            this.tb5.TabIndex = 25;
            this.tb5.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb3
            // 
            this.tb3.Enabled = false;
            this.tb3.LargeChange = 1;
            this.tb3.Location = new System.Drawing.Point(320, 104);
            this.tb3.Maximum = 255;
            this.tb3.Name = "tb3";
            this.tb3.Size = new System.Drawing.Size(95, 45);
            this.tb3.TabIndex = 24;
            this.tb3.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // tb1
            // 
            this.tb1.Enabled = false;
            this.tb1.LargeChange = 1;
            this.tb1.Location = new System.Drawing.Point(421, 104);
            this.tb1.Maximum = 255;
            this.tb1.Name = "tb1";
            this.tb1.Size = new System.Drawing.Size(95, 45);
            this.tb1.TabIndex = 23;
            this.tb1.TickStyle = System.Windows.Forms.TickStyle.None;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(537, 150);
            this.Controls.Add(this.tb9);
            this.Controls.Add(this.tb7);
            this.Controls.Add(this.tb5);
            this.Controls.Add(this.tb3);
            this.Controls.Add(this.tb1);
            this.Controls.Add(this.tb8);
            this.Controls.Add(this.tb6);
            this.Controls.Add(this.tb4);
            this.Controls.Add(this.tb2);
            this.Controls.Add(this.tb0);
            this.Controls.Add(this.tbLine2);
            this.Controls.Add(this.tbLine1);
            this.Controls.Add(this.btnConnect);
            this.Controls.Add(this.cbPorts);
            this.Controls.Add(this.btnRescan);
            this.Controls.Add(this.btnSend);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "Form1";
            this.Text = "USB I/O Test";
            this.Load += new System.EventHandler(this.Form1_Load);
            ((System.ComponentModel.ISupportInitialize)(this.tb0)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb4)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb6)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb8)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb9)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb7)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb5)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb3)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.tb1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.ComboBox cbPorts;
        private System.Windows.Forms.Button btnRescan;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.TextBox tbLine1;
        private System.Windows.Forms.TextBox tbLine2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.TrackBar tb0;
        private System.Windows.Forms.TrackBar tb2;
        private System.Windows.Forms.TrackBar tb4;
        private System.Windows.Forms.TrackBar tb6;
        private System.Windows.Forms.TrackBar tb8;
        private System.Windows.Forms.TrackBar tb7;
        private System.Windows.Forms.TrackBar tb5;
        private System.Windows.Forms.TrackBar tb3;
        private System.Windows.Forms.TrackBar tb1;
        private System.Windows.Forms.TrackBar tb9;
    }
}

