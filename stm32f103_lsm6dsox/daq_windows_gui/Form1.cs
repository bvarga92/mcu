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

namespace lsm6dsox_gui
{
    public partial class Form1 : Form
    {

        private const int dx = 3;
        private SerialPort comPort = new SerialPort();
        private double temperature;
        private double[,] buf;
        private int bufEnd = -1;
        private int bufValidCnt = 0;
        private Bitmap bmAcc, bmGyr;
        private Graphics gAcc, gGyr;
        private System.IO.StreamWriter file;
        private Semaphore comPortSemaphore = new Semaphore(1, 1);
        private bool sampling = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void findPorts()
        {
            string[] list = SerialPort.GetPortNames();
            cbPorts.Items.Clear();
            if (list.Length == 0) return;
            for (int i = 0; i <= list.GetUpperBound(0); i++) cbPorts.Items.Add(list[i]);
            Array.Sort(list);
            cbPorts.Text = list[0];
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
            findPorts();
            bmAcc = new Bitmap(pictureBoxAcc.ClientSize.Width, pictureBoxAcc.ClientSize.Height);
            bmGyr = new Bitmap(pictureBoxGyr.ClientSize.Width, pictureBoxGyr.ClientSize.Height);
            gAcc = Graphics.FromImage(bmAcc);
            gGyr = Graphics.FromImage(bmGyr);
            gAcc.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            gGyr.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            buf = new double[6, pictureBoxAcc.ClientSize.Width / dx + 1];
            labelAcc.Left = (this.ClientSize.Width - labelAcc.Size.Width) / 2;
            labelGyr.Left = (this.ClientSize.Width - labelGyr.Size.Width) / 2;
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
                if (cbPorts.Items.Count == 0) return;
                comPort.PortName = Convert.ToString(cbPorts.Text);
                comPort.BaudRate = 9600;
                comPort.DataBits = 8;
                comPort.StopBits = (StopBits)Enum.Parse(typeof(StopBits), "One");
                comPort.Handshake = (Handshake)Enum.Parse(typeof(Handshake), "None");
                comPort.Parity = (Parity)Enum.Parse(typeof(Parity), "None");
                comPort.DataReceived += new SerialDataReceivedEventHandler(dataReceivedHandler);
                btnConnect.BackColor = Color.SpringGreen;
                btnConnect.Text = "Disconnect";
                chbLog.Enabled = false;
                sampling = true;
                try
                {
                    if (chbLog.Checked)
                    {
                        file = new System.IO.StreamWriter("lsm6dsox_" + DateTime.Now.ToString("yyyy_MM_dd_HH_mm_ss") + ".txt");
                        file.WriteLine("A_X [mg]\tA_Y [mg]\tA_Z [mg]\tG_X [dps]\tG_Y [dps]\tG_Z [dps]");
                    }
                    comPort.Open();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else
            {
                comPortSemaphore.WaitOne();
                sampling = false;
                if (comPort.IsOpen) (new Thread(() => { comPort.Close(); })).Start();
                btnConnect.BackColor = SystemColors.Control;
                btnConnect.Text = "Connect";
                if (chbLog.Checked) file.Close();
                chbLog.Enabled = true;
                comPortSemaphore.Release();
            }
        }

        private void dataReceivedHandler(object sender, SerialDataReceivedEventArgs e)
        {
            System.Threading.Thread.CurrentThread.CurrentCulture = new System.Globalization.CultureInfo("en-US");
            comPortSemaphore.WaitOne();
            if (!sampling)
            {
                comPortSemaphore.Release();
                return;
            }
            try
            {
                string str = ((SerialPort)sender).ReadTo("$");
                temperature = 25 + Convert.ToInt16(str.Substring(1, 4), 16) / 256.0;
                bufEnd = (bufEnd + 1) % buf.GetLength(1);
                buf[0, bufEnd] = Convert.ToInt16(str.Substring(6, 4), 16) * 16000.0 / 32767.0; //accX
                buf[1, bufEnd] = Convert.ToInt16(str.Substring(11, 4), 16) * 16000.0 / 32767.0; //accY
                buf[2, bufEnd] = Convert.ToInt16(str.Substring(16, 4), 16) * 16000.0 / 32767.0; //accZ
                buf[3, bufEnd] = Convert.ToInt16(str.Substring(21, 4), 16) * 2000.0 / 32767.0; //gyrX
                buf[4, bufEnd] = Convert.ToInt16(str.Substring(26, 4), 16) * 2000.0 / 32767.0; //gyrY
                buf[5, bufEnd] = Convert.ToInt16(str.Substring(31, 4), 16) * 2000.0 / 32767.0; //gyrZ
                if (bufValidCnt < buf.GetLength(1)) bufValidCnt++;
                if (chbLog.Checked) file.WriteLine("{0, 0:E5}\t{1, 1:E5}\t{2, 2:E5}\t{3, 3:E5}\t{4, 4:E5}\t{5, 5:E5}", buf[0, bufEnd], buf[1, bufEnd], buf[2, bufEnd], buf[3, bufEnd], buf[4, bufEnd], buf[5, bufEnd]);
                BeginInvoke(new Action(() => { plotData(); }));
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
            comPortSemaphore.Release();
        }

        private void plotData()
        {
            labelTemp.Text = String.Format("Temperature: {0, 0:f1}\u00B0C", temperature);
            gAcc.Clear(SystemColors.Control);
            gGyr.Clear(SystemColors.Control);
            int x = (buf.GetLength(1) - bufValidCnt) * dx;
            double[] y = { 0, 0, 0, 0, 0, 0 }, yPrev = { 0, 0, 0, 0, 0, 0 };
            for (int i = buf.GetLength(1) - bufValidCnt; i < buf.GetLength(1); i++, x += dx)
            {
                for (int j = 0; j < 6; j++) y[j] = buf[j, (bufEnd + 1 + i) % buf.GetLength(1)];
                if (i > buf.GetLength(1) - bufValidCnt)
                {
                    gAcc.DrawLine(new Pen(System.Drawing.Color.Red, 2), x - dx, 65 - (int)(yPrev[0] * 0.022), x, 65 - (int)(y[0] * 0.022));
                    gAcc.DrawLine(new Pen(System.Drawing.Color.Green, 2), x - dx, 65 - (int)(yPrev[1] * 0.022), x, 65 - (int)(y[1] * 0.022));
                    gAcc.DrawLine(new Pen(System.Drawing.Color.Blue, 2), x - dx, 65 - (int)(yPrev[2] * 0.022), x, 65 - (int)(y[2] * 0.022));
                    gGyr.DrawLine(new Pen(System.Drawing.Color.Red, 2), x - dx, 65 - (int)(yPrev[3] / 10.0), x, 65 - (int)(y[3] / 10.0));
                    gGyr.DrawLine(new Pen(System.Drawing.Color.Green, 2), x - dx, 65 - (int)(yPrev[4] / 10.0), x, 65 - (int)(y[4] / 10.0));
                    gGyr.DrawLine(new Pen(System.Drawing.Color.Blue, 2), x - dx, 65 - (int)(yPrev[5] / 10.0), x, 65 - (int)(y[5] / 10.0));
                }
                for (int j = 0; j < 6; j++) yPrev[j] = y[j];
            }
            pictureBoxAcc.Image = bmAcc;
            pictureBoxGyr.Image = bmGyr;
        }

    }
}
