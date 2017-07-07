namespace stepper
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
            this.btnTransistor = new System.Windows.Forms.Button();
            this.btnRelay = new System.Windows.Forms.Button();
            this.tbMot1 = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.tbMot2 = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.tbMot3 = new System.Windows.Forms.TextBox();
            this.btnMot1 = new System.Windows.Forms.Button();
            this.btnMot2 = new System.Windows.Forms.Button();
            this.btnMot3 = new System.Windows.Forms.Button();
            this.cbDriveMode = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
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
            this.btnRescan.Location = new System.Drawing.Point(98, 10);
            this.btnRescan.Name = "btnRescan";
            this.btnRescan.Size = new System.Drawing.Size(75, 23);
            this.btnRescan.TabIndex = 1;
            this.btnRescan.Text = "Rescan";
            this.btnRescan.UseVisualStyleBackColor = true;
            this.btnRescan.Click += new System.EventHandler(this.btnRescan_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Location = new System.Drawing.Point(183, 10);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(75, 23);
            this.btnConnect.TabIndex = 2;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnTransistor
            // 
            this.btnTransistor.Location = new System.Drawing.Point(12, 51);
            this.btnTransistor.Name = "btnTransistor";
            this.btnTransistor.Size = new System.Drawing.Size(91, 23);
            this.btnTransistor.TabIndex = 3;
            this.btnTransistor.Text = "Transistor On";
            this.btnTransistor.UseVisualStyleBackColor = true;
            this.btnTransistor.Click += new System.EventHandler(this.btnTransistor_Click);
            // 
            // btnRelay
            // 
            this.btnRelay.Location = new System.Drawing.Point(115, 51);
            this.btnRelay.Name = "btnRelay";
            this.btnRelay.Size = new System.Drawing.Size(91, 23);
            this.btnRelay.TabIndex = 4;
            this.btnRelay.Text = "Relay On";
            this.btnRelay.UseVisualStyleBackColor = true;
            this.btnRelay.Click += new System.EventHandler(this.btnRelay_Click);
            // 
            // tbMot1
            // 
            this.tbMot1.Location = new System.Drawing.Point(65, 118);
            this.tbMot1.MaxLength = 5;
            this.tbMot1.Name = "tbMot1";
            this.tbMot1.Size = new System.Drawing.Size(52, 20);
            this.tbMot1.TabIndex = 5;
            this.tbMot1.Text = "1000";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 121);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(46, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Motor 1:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(9, 147);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(46, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "Motor 2:";
            // 
            // tbMot2
            // 
            this.tbMot2.Location = new System.Drawing.Point(65, 144);
            this.tbMot2.MaxLength = 5;
            this.tbMot2.Name = "tbMot2";
            this.tbMot2.Size = new System.Drawing.Size(52, 20);
            this.tbMot2.TabIndex = 7;
            this.tbMot2.Text = "1000";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(9, 173);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 14;
            this.label3.Text = "Motor 3:";
            // 
            // tbMot3
            // 
            this.tbMot3.Location = new System.Drawing.Point(65, 170);
            this.tbMot3.MaxLength = 5;
            this.tbMot3.Name = "tbMot3";
            this.tbMot3.Size = new System.Drawing.Size(52, 20);
            this.tbMot3.TabIndex = 9;
            this.tbMot3.Text = "1000";
            // 
            // btnMot1
            // 
            this.btnMot1.Location = new System.Drawing.Point(131, 115);
            this.btnMot1.Name = "btnMot1";
            this.btnMot1.Size = new System.Drawing.Size(75, 23);
            this.btnMot1.TabIndex = 15;
            this.btnMot1.Text = "Move";
            this.btnMot1.UseVisualStyleBackColor = true;
            this.btnMot1.Click += new System.EventHandler(this.btnMot1_Click);
            // 
            // btnMot2
            // 
            this.btnMot2.Location = new System.Drawing.Point(131, 143);
            this.btnMot2.Name = "btnMot2";
            this.btnMot2.Size = new System.Drawing.Size(75, 23);
            this.btnMot2.TabIndex = 16;
            this.btnMot2.Text = "Move";
            this.btnMot2.UseVisualStyleBackColor = true;
            this.btnMot2.Click += new System.EventHandler(this.btnMot2_Click);
            // 
            // btnMot3
            // 
            this.btnMot3.Location = new System.Drawing.Point(131, 167);
            this.btnMot3.Name = "btnMot3";
            this.btnMot3.Size = new System.Drawing.Size(75, 23);
            this.btnMot3.TabIndex = 17;
            this.btnMot3.Text = "Move";
            this.btnMot3.UseVisualStyleBackColor = true;
            this.btnMot3.Click += new System.EventHandler(this.btnMot3_Click);
            // 
            // cbDriveMode
            // 
            this.cbDriveMode.FormattingEnabled = true;
            this.cbDriveMode.Items.AddRange(new object[] {
            "Wave drive",
            "Half step",
            "Full step"});
            this.cbDriveMode.Location = new System.Drawing.Point(85, 88);
            this.cbDriveMode.Name = "cbDriveMode";
            this.cbDriveMode.Size = new System.Drawing.Size(121, 21);
            this.cbDriveMode.TabIndex = 18;
            this.cbDriveMode.SelectedIndex = 1;
            this.cbDriveMode.SelectedIndexChanged += new System.EventHandler(this.cbDriveMode_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(9, 91);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(64, 13);
            this.label4.TabIndex = 19;
            this.label4.Text = "Drive mode:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(269, 203);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.cbDriveMode);
            this.Controls.Add(this.btnMot3);
            this.Controls.Add(this.btnMot2);
            this.Controls.Add(this.btnMot1);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.tbMot3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.tbMot2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tbMot1);
            this.Controls.Add(this.btnRescan);
            this.Controls.Add(this.btnRelay);
            this.Controls.Add(this.btnTransistor);
            this.Controls.Add(this.cbPorts);
            this.Controls.Add(this.btnConnect);
            this.Name = "Form1";
            this.Text = "Stepper Test";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.ComboBox cbPorts;
        private System.Windows.Forms.Button btnTransistor;
        private System.Windows.Forms.Button btnRelay;
        private System.Windows.Forms.Button btnRescan;
        private System.Windows.Forms.TextBox tbMot1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox tbMot3;
        private System.Windows.Forms.Button btnMot1;
        private System.Windows.Forms.Button btnMot2;
        private System.Windows.Forms.Button btnMot3;
        private System.Windows.Forms.TextBox tbMot2;
        private System.Windows.Forms.ComboBox cbDriveMode;
        private System.Windows.Forms.Label label4;
    }
}

