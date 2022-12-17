using System;
using Xamarin.Forms;

namespace VPenExample
{
    public partial class MainPage : ContentPage
    {
        public MainPage()
        {
            InitializeComponent();
            this.BindingContext = ViewModel = new MainPageViewModel();
        }

        public MainPageViewModel ViewModel { get; }

        protected override void OnAppearing()
        {
            if (!BluetoothController.LeScanner.IsRunning)
                BluetoothController.LeScanner.Restart();
            BluetoothController.LeScanner.NewData += LeScanner_NewData;
            base.OnAppearing();
        }

        private void LeScanner_NewData(object sender, BleDataEventArgs e)
        {
            Device.BeginInvokeOnMainThread(() => ViewModel.OnNewData(e));
        }

        protected override void OnDisappearing()
        {
            BluetoothController.LeScanner.NewData -= LeScanner_NewData;
            base.OnDisappearing();
        }

        private async void Button_Clicked(object sender, System.EventArgs e)
        {
            try
            {
                plot.Model = await ViewModel.Start();
            }
            catch (Exception exc)
            {
                await DisplayAlert(this.Title, exc.Message, "Ok");
            }
        }
    }
}