using OxyPlot;
using OxyPlot.Axes;
using OxyPlot.Series;
using System;
using System.Threading.Tasks;

namespace VPenExample
{
    public class MainPageViewModel : BaseViewModel
    {
        public void OnNewData(BleDataEventArgs e)
        {
            try
            {
                this.Device = e.Data.Address;
                var data = e.Data.Dara.BytesToStruct<ViPenAdvertising>();
                Velocity = Math.Round(data.Velocity * 0.01, 2);
                Value = Math.Round(data.Acceleration * 0.1, 2);
                Kurtosis = Math.Round(data.Kurtosis * 0.01, 2);
                Temperature = Math.Round(data.Temperature * 0.01, 2);
                LastAdvertisind = DateTime.Now;
            }
            catch { return; }
        }
        private void SetValues(VPen2Data data)
        {
            if (data == null) return;
            Velocity = data.Header.Header.Values[0] * 0.01;
            Value = data.Header.Header.Values[1] * 0.1;

            Kurtosis = data.Header.Header.Values[2] * 0.01;
            Temperature = data.Header.Header.Values[3] * 0.01;
        }
        private DateTime _lastAdvertisind;
        public DateTime LastAdvertisind { get => _lastAdvertisind; set { _lastAdvertisind = value; OnPropertyChanged(); } }

        private double velocity;
        public double Velocity { get => velocity; set { velocity = value; OnPropertyChanged(); } }
        private double value;
        public double Value { get => value; set { this.value = value; OnPropertyChanged(); } }
        private double kurtosis;
        public double Kurtosis { get => kurtosis; set { kurtosis = value; OnPropertyChanged(); } }
        private double temperature;
        public double Temperature { get => temperature; set { temperature = value; OnPropertyChanged(); } }
        public string Device { get; set; }

        public OperationToken Token { get; } = new OperationToken();

        public async Task<bool> StartMeasurement(IVPen2Control controller)
        {
            try
            {
                this.IsBusy = true;

                if (!controller.IsConnected)
                    await controller.ConnectAsync(Device, Token);
                if (!controller.IsConnected)
                    throw new Exception("GATT connection error!");

                return await controller.StartMeasurementAsync(Device, Token, TVipen2MeasureSetup.MeasurementRegular().StructToBytes());
            }
            finally
            {
                this.IsBusy = false;
            }
        }

        public async Task<VPen2Data> GetWaveform(IVPen2Control controller)
        {
            try
            {
                this.IsBusy = true;
                return await controller.Download(Device, Token);
            }
            finally
            {
                this.IsBusy = false;
            }
        }


        public async Task<PlotModel> Start()
        {
            if (string.IsNullOrEmpty(Device))
                throw new Exception("Can't find ViPen device!");

            VPen2Data blocks = null;
            using (var controller = VPen2ControlManager.GetController())
            {
                try
                {
                    if (await StartMeasurement(controller))
                    {
                        blocks = await GetWaveform(controller);
                    }
                }
                finally
                {
                    await controller.Disconnect();
                }
            }
            var wav = await blocks.CheckAndConvert();
            SetValues(blocks);
            var model = new PlotModel();

            var yAxis = new LinearAxis();

            yAxis.Position = AxisPosition.Left;
            yAxis.MajorGridlineStyle = LineStyle.Solid;
            yAxis.MinorGridlineStyle = LineStyle.Solid;
            yAxis.MinorGridlineColor = OxyColor.FromArgb(128, 211, 211, 211);

            yAxis.TextColor = OxyColors.Red;
            yAxis.TickStyle = TickStyle.Inside;

            model.Axes.Add(yAxis);

            var xAxis = new LinearAxis();
            xAxis.MajorGridlineStyle = LineStyle.Solid;
            xAxis.MinorGridlineStyle = LineStyle.Solid;
            xAxis.MinorGridlineColor = OxyColor.FromArgb(128, 211, 211, 211);
            xAxis.Position = AxisPosition.Bottom;
            xAxis.TextColor = OxyColors.Blue;

            xAxis.TickStyle = TickStyle.Inside;
            var line = new LineSeries() { Decimator = Decimator.Decimate, StrokeThickness = 2, Color = OxyColors.Red };
            for (int i = 0; i < blocks.Header.Header.DataLen; i++)
            {
                line.Points.Add(new DataPoint(i * blocks.Header.Header.DataDX, wav[i]));
            }

            model.Series.Add(line);

            return model;
        }
    }
}