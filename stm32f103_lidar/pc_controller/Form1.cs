using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace lidarcontroller
{
    public partial class Form1 : Form
    {
        private SerialPort comPort = new SerialPort();
        private volatile bool readFlag = false, scanning = false, stopScan = false;
        private System.IO.StreamWriter file;

        public Form1()
        {
            InitializeComponent();
        }

        private void findPorts()
        {
            string[] list = SerialPort.GetPortNames();
            cbPorts.Items.Clear();
            for (int i = 0; i <= list.GetUpperBound(0); i++) cbPorts.Items.Add(list[i]);
            Array.Sort(list);
            if (list.GetLength(0) > 0)
            {
                cbPorts.Text = list[0];
                btnConnect.Enabled = true;
            }
            else
            {
                cbPorts.Text = "";
                btnConnect.Enabled = false;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            findPorts();
        }

        private void btnRescan_Click(object sender, EventArgs e)
        {
            findPorts();
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            if (btnConnect.Text.Equals("Connect"))
            {
                if (comPort.IsOpen) comPort.Close();
                comPort.PortName = Convert.ToString(cbPorts.Text);
                comPort.BaudRate = 9600;
                comPort.DataBits = 8;
                comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "One");
                comPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), "None");
                comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
                comPort.DataReceived += new SerialDataReceivedEventHandler(dataReceivedHandler);
                try
                {
                    comPort.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
                btnConnect.BackColor = Color.SpringGreen;
                btnConnect.Text = "Disconnect";
                btnCenter.Enabled = btnUp.Enabled = btnLeft.Enabled = btnDown.Enabled = btnRight.Enabled = btnFreeRun.Enabled = btnStartScan.Enabled = true;
                btnRescan.Enabled = false;
            }
            else
            {
                if (comPort.IsOpen) (new Thread(() => { while (readFlag) ; comPort.Close(); })).Start();
                btnConnect.BackColor = SystemColors.Control;
                btnConnect.Text = "Connect";
                btnCenter.Enabled = btnUp.Enabled = btnLeft.Enabled = btnDown.Enabled = btnRight.Enabled = btnFreeRun.Enabled = btnStartScan.Enabled = false;
                btnRescan.Enabled = true;
           }
        }

        private void dataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            if (comPort.IsOpen)
            {
                try
                {
                    readFlag = true;
                    string str = ((SerialPort)sender).ReadTo("$");
                    readFlag = false;
                    if (scanning) file.WriteLine(str);
                    MethodInvoker inv = delegate { this.labelDst.Text = str + " cm"; };
                    this.Invoke(inv);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void btnCenter_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("c");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnUp_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("u");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnLeft_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("l");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnDown_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("d");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnRight_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("r");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnFreeRun_Click(object sender, EventArgs e)
        {
            try
            {
                comPort.Write("f");
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void btnStartScan_Click(object sender, EventArgs e)
        {
            if (btnStartScan.Text.Equals("Start Scan"))
            {
                btnStartScan.Text = "Stop Scan";
                btnCenter.Enabled = btnUp.Enabled = btnLeft.Enabled = btnDown.Enabled = btnRight.Enabled = btnFreeRun.Enabled = btnConnect.Enabled = false;
                file = new System.IO.StreamWriter("./scandata.txt");
                (new Thread(() => { scan(); })).Start();
            }
            else
            {
                stopScan = true;
            }
        }

        private void scan()
        {
            try
            {
                comPort.Write("s");
                comPort.Write("c");
                for (int i = 0; i < 15; i++) { comPort.Write("r"); Thread.Sleep(20); }
                for (int i = 0; i < 10; i++) { comPort.Write("u"); Thread.Sleep(20); }
                comPort.DiscardInBuffer();
                comPort.BaseStream.Flush();
                scanning = true;
                bool dir = false;
                for (int i = 0; i < 15; i++)
                {
                    for (int j = 0; j < 30; j++)
                    {
                        if (stopScan) break;
                        comPort.Write("s");
                        Thread.Sleep(250);
                        if (j < 29)
                        {
                            comPort.Write(dir ? "r" : "l");
                            Thread.Sleep(700);
                        }
                    }
                    if (stopScan) break;
                    dir = !dir;
                    comPort.Write("d");
                    Thread.Sleep(700);
                }
                comPort.Write("c");
                MethodInvoker inv = delegate {
                    this.file.Close();
                    this.btnStartScan.Text = "Start Scan";
                    this.btnCenter.Enabled = this.btnUp.Enabled = this.btnLeft.Enabled = this.btnDown.Enabled = this.btnRight.Enabled = this.btnFreeRun.Enabled = this.btnConnect.Enabled = true;
                };
                this.Invoke(inv);
                stopScan = false;
                scanning = false;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

    }
}
