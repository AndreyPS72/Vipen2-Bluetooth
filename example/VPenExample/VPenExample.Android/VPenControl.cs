using Android.Bluetooth;
using Java.Util;
using System;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Xamarin.Forms;

[assembly: Dependency(typeof(VPenExample.Droid.VPen2Control))]

namespace VPenExample.Droid
{
    public class VPen2Control : IVPen2Control
    {
        private static BluetoothAdapter GetAdapter()
        {
            BluetoothAdapter adapter = BluetoothAdapter.DefaultAdapter;

            if (adapter == null)
                throw new Exception("No Bluetooth adapter found.");

            if (!adapter.IsEnabled)
            {
                adapter.Enable();

                for (int i = 0; i < 64; i++)
                {
                    if (adapter.State == State.On)
                        break;
                    else
                        Thread.Sleep(10);
                }
            }

            if (!adapter.IsEnabled)
                throw new Exception("Bluetooth adapter is disabled.");
            return adapter;
        }

        private static BluetoothDevice FindBluetoothDevice(string deviceAddress)
        {
            var adapter = GetAdapter();

            var bytes = deviceAddress.Trim().Split(':').Select(s => Convert.ToByte(s, 16)).ToArray();

            BluetoothDevice device = adapter.GetRemoteDevice(bytes);
            return device;
        }

        private bool GetConnected()
        {
            try
            {
                return Gatt != null && Gatt.IsGattConnected;
            }
            catch
            {
                return false;
            }
        }

        public bool IsConnected => GetConnected();

        public GATTConetionTools Gatt { get; private set; }

        public bool LongFrame { get; private set; }

        public async Task<bool> ConnectAsync(string deviceAddress, OperationToken token)
        {
            try
            {
                await Disconnect();
                Dispose();

                if (Gatt == null)
                    Gatt = new GATTConetionTools();
                var device = await Task.Run(() => FindBluetoothDevice(deviceAddress));
                for (int i = 0; i < 4; i++)
                {
                    try
                    {
                        if (await Gatt.ConnectGattAsync(device, TimeSpan.FromSeconds(10), false))
                            break;
                    }
                    catch { continue; }
                }
                LongFrame = false;
                if ((await Gatt.ChangeMtuAsync(TimeSpan.FromSeconds(5), 512)) > 100)
                    LongFrame = true;

                if (!await Gatt.DiscoverServicesAsync(TimeSpan.FromSeconds(10)))
                    throw new Exception("Cant discover services!");

                if (IsConnected)
                    Gatt.Gatt.RequestConnectionPriority(GattConnectionPriority.High);

                return IsConnected;
            }
            catch (Exception exc)
            {
                Console.WriteLine(exc.StackTrace);
                return false;
            }
        }

        public async Task Disconnect()
        {
            try
            {
                await Gatt?.DisconnectGattAsync(TimeSpan.FromSeconds(5));
                Dispose();
            }
            catch { return; }
            finally
            {
                Gatt = null;
            }
        }

        public void Dispose()
        {
            try
            {
                Gatt?.Deinit();
            }
            catch { return; }
            finally
            {
                Gatt = null;
            }
        }

        private static readonly UUID SERVICE = UUID.FromString("413557AA-213F-4279-8530-D38E41390000");

        private static readonly UUID C_READING = UUID.FromString("42EC1288-B8A0-43DB-AE00-29F942ED0001");
        private static readonly UUID C_CTRL = UUID.FromString("42EC1288-B8A0-43DB-AE00-29F942ED0002");

        private static readonly UUID C_WAV_CTRL = UUID.FromString("42EC1288-B8A0-43DB-AE00-29F942ED0003");
        private static readonly UUID C_WAV_READ = UUID.FromString("42EC1288-B8A0-43DB-AE00-29F942ED0004");

        private const ushort ViPen_State_Stoped = (0 << 0);
        private const ushort ViPen_State_Started = (1 << 0);
        private const ushort ViPen_State_NoData = (0 << 1);
        private const ushort ViPen_State_Data = (1 << 1);

        private const byte ViPen_Get_Data_Vel = 0x10;

        public async Task<bool> StartMeasurementAsync(string deviceAddress, OperationToken token, byte[] setudata)
        {
            if (!IsConnected)
                throw new Exception("GATT is not connected!");

            var service = Gatt.Gatt.GetService(SERVICE);
            if (service == null)
                throw new Exception("Service discover error!");

            var cControl = service.GetCharacteristic(C_CTRL);
            if (cControl == null)
                throw new Exception("Cant find characteristics!");

            if (IsDownloading)
            {
                if (token.IsAborted)
                    return false;

                for (int i = 0; i < 40; i++)
                {
                    if (IsDownloading)
                        await Task.Delay(100);
                    else
                        break;
                }
            }

            if (!await Gatt.Write(setudata, cControl, TimeSpan.FromSeconds(5), true))
                throw new Exception("Characteristics set error!");

            return true;
        }

        public async Task<bool> CustomCommand(string deviceAddress, string serviceId, string charId, byte[] payload)
        {
            if (!IsConnected)
                throw new Exception("GATT is not connected!");
            using (var SID = UUID.FromString(serviceId))
            using (var CID = UUID.FromString(charId))
            {
                var service = Gatt.Gatt.GetService(SID);
                if (service == null)
                    throw new Exception("Service discover error!");

                var cControl = service.GetCharacteristic(CID);
                if (cControl == null)
                    throw new Exception("Cant find characteristics!");

                await Gatt.Write(payload, cControl, TimeSpan.FromSeconds(2), true);
                return true;
            }
        }

        private static int CalculateBlockCount(byte[] data)
        {
            var header = data.BytesToStruct<stVPen2Header>();

            return header.ViPen2_Data_Blocks;
        }

        public bool IsDownloading { get; private set; }

        public async Task<VPen2Data> Download(string deviceAddress, OperationToken token)
        {
            IsDownloading = true;
            if (!IsConnected)
                throw new Exception("GATT is not connected!");

            var service = Gatt.Gatt.GetService(SERVICE);
            if (service == null)
                throw new Exception("Service discover error!");

            var cControl = service.GetCharacteristic(C_WAV_CTRL);
            if (cControl == null)
                throw new Exception("Cant find characteristics!");

            var cRead = service.GetCharacteristic(C_WAV_READ);
            if (cRead == null)
                throw new Exception("Cant find characteristics!");

            var cStatus = service.GetCharacteristic(C_CTRL);
            if (cStatus == null)
                throw new Exception("Cant find characteristics!");
            try
            {
                var watch = new Stopwatch();
                watch.Start();
                bool timedout = true;
                int readerrorcount = 0;
                while (watch.ElapsedMilliseconds < 33000)
                {
                    try
                    {
                        if (token.IsAborted)
                            return null;

                        var data = await Gatt.Read(cStatus, TimeSpan.FromSeconds(2), true);
                        readerrorcount = 0;
                        var status = BitConverter.ToUInt16(data, 0);
                        if ((status & ViPen_State_Data) > 0)
                        {
                            timedout = false;
                            break;
                        }
                    }
                    catch
                    {
                        readerrorcount++;
                        if (readerrorcount > 3)
                            throw;
                    }
                }

                if (timedout)
                    throw new TimeoutException();

                const int BLOCKSIZE = 236;

                if (!await Gatt.SubscribeAsync(cRead, true, TimeSpan.FromSeconds(5), true))
                    throw new Exception("Subscription error!");

                var result = new VPen2Data();
                var request = new byte[]
                     {
                            ViPen_Get_Data_Vel,
                            0
                     };

                if (token.IsAborted)
                    return null;

                var writeTask = Gatt.Write(request, cControl, TimeSpan.FromSeconds(2), false);
                var readTask = Gatt.ReadBlockWithNotificationWithHeaderAsync(C_WAV_READ, TimeSpan.FromSeconds(30), token, BLOCKSIZE, CalculateBlockCount);
                await Task.WhenAll(writeTask, readTask);

                if (token.IsAborted)
                    return null;

                if (!writeTask.Result)
                    throw new Exception("Characteristics set error!");

                if (readTask.Result == null)
                    throw new Exception("Blocks read error!");

                var all = readTask.Result;
                result.Header = all.BytesToStruct<stVPen2Data>();
                return result;
            }
            catch
            {
                if (!token.IsAborted)
                    throw;

                return null;
            }
            finally
            {
                await Unsubscribe(cRead);
                IsDownloading = false;
            }
        }

        private async Task Unsubscribe(BluetoothGattCharacteristic cRead)
        {
            try
            {
                await Gatt?.SubscribeAsync(cRead, false, TimeSpan.FromSeconds(5), true);
            }
            catch { return; }
        }
    }
}