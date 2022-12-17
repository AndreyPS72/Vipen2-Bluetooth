using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using Xamarin.Forms;

namespace VPenExample
{
    public class OperationToken : INotifyPropertyChanged
    {
        public bool IsAborted { get; set; } = false;

        private double _progress;

        public double Progress { get => _progress; set { _progress = value; OnPropertyChanged(); } }

        public event PropertyChangedEventHandler PropertyChanged;

        protected void OnPropertyChanged([CallerMemberName] string propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
    public interface IVPen2Control : IDisposable
    {
        Task<bool> ConnectAsync(string connection, OperationToken token);

        Task<bool> StartMeasurementAsync(string connection, OperationToken token, byte[] setup);

        Task<VPen2Data> Download(string connection, OperationToken token);

        Task Disconnect();

        bool IsConnected { get; }

        Task<bool> CustomCommand(string connection, string serviceId, string charId, byte[] payload);
    }

    public static class VPen2ControlManager
    {
        public static IVPen2Control GetController()
        {
            return DependencyService.Get<IVPen2Control>(DependencyFetchTarget.NewInstance);
        }
    }

    public class VPen2Data
    {
        public stVPen2Data Header { get; set; }

        public async Task<float[]> CheckAndConvert()
        {
            return await Task.Run(() =>
            {
                var timestamp = Header.Header.Timestamp;
                var factor = Header.Header.Coeff;
                var items = new List<short>();

                for (int i = 0; i < Header.Header.ViPen2_Data_Blocks - 1; i++)
                {
                    var block = Header.Blocks[i];
                    if (block.ViPen_Get_Wave_ID != Header.Header.ViPen2_Get_Wave_ID)
                        throw new Exception("Incorrect timestamp!");

                    items.AddRange(block.Data);
                }

                var result = new float[items.Count];

                for (int i = 0; i < result.Length; i++)
                {
                    result[i] = items[i] * factor;
                }

                return result;
            });
        }
    }

    public enum VIPEN2_BT_COMMAND : uint
    {
        NONE = (0),
        START = (1),
        STOP = (2),
        IDLE = (3),
        OFF = (4),
        CALIBRATION_0 = (0x10),
        CALIBRATION_20 = (0x11),
        CALIBRATION_1KHZ = (0x20),
        CALIBRATION_SAVE = (0x30),
        TEST = (0x80),
        SET_DATA = (0x40)
    }

    public enum MEAS_TYPE : uint
    {
        SPECTRUM = (0),
        WAVEFORM = (1),
        SPECTRUM_SLOW = (2),
        WAVEFORM_SLOW = (3),
        SPECTRUM_ENV = (4),
        WAVEFORM_ENV = (5)
    }

    public enum MEAS_UNITS : uint
    {
        ACCELERATION = (0),
        VELOCITY = (1),
        DISPLACEMENT = (2),
    }

    public enum SETUP_DX : uint
    {
        DX_256_HZ = (0),
        DX_640_HZ = (1),
        DX_2560_HZ = (2),
        DX_6400_HZ = (3),
        DX_25600_HZ = (4),
    }

    public enum SETUP_ALLX : uint
    {
        ALLX_256 = (0),
        ALLX_1K = (1),
        ALLX_2K = (2),
        ALLX_8K = (3),
    }

    public enum SETUP_AVG
    {
        AVG_NO = (0),
        AVG_4_STOP = (1),
        AVG_10_STOP = (2),
        AVG_999 = (3),
    }

    public struct TVipen2MeasureSetup
    {
        public const int MEAS_TYPE_SPECTRUM_MASK = (0);
        public const int MEAS_TYPE_WAVEFORM_MASK = (1);

        public const int SETUP_EXTERNAL_SENSOR = (0);
        public const int SETUP_INTERNAL_DAC = (1);

        public const int SETUP_READ_MODE = (0);
        public const int SETUP_CALIBRATION_MODE = (1);

        public VIPEN2_BT_COMMAND Command;

        public MEAS_TYPE MeasType;
        public MEAS_UNITS MeasUnits;

        public SETUP_ALLX AllX;
        public SETUP_DX dX;

        public SETUP_AVG Avg;

        public uint InternalDAC;
        public uint CalibrationMode;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 8)]
        public uint[] Reserv;

        public static TVipen2MeasureSetup MeasurementRegular()
        {
            var aBluetoothSetupFromCommand = Create();

            aBluetoothSetupFromCommand.Command = VIPEN2_BT_COMMAND.START;
            aBluetoothSetupFromCommand.MeasType = MEAS_TYPE.SPECTRUM;
            aBluetoothSetupFromCommand.MeasUnits = MEAS_UNITS.VELOCITY;
            aBluetoothSetupFromCommand.dX = SETUP_DX.DX_25600_HZ;
            aBluetoothSetupFromCommand.AllX = SETUP_ALLX.ALLX_8K;
            aBluetoothSetupFromCommand.Avg = SETUP_AVG.AVG_4_STOP;

            return aBluetoothSetupFromCommand;
        }


        public static TVipen2MeasureSetup Stop()
        {
            var aBluetoothSetupFromCommand = Create();

            aBluetoothSetupFromCommand.Command = VIPEN2_BT_COMMAND.STOP;
            return aBluetoothSetupFromCommand;
        }

        private static TVipen2MeasureSetup Create()
        {
            return new TVipen2MeasureSetup() { Reserv = new uint[8] };
        }
    }

    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct stVPen2Header
    {
        public const int VIPEN2_BLUETOOTH_PACKET_DATA_SIZE = 236;
        public byte ViPen2_Get_Data_Command;
        public byte ViPen2_Get_Data_Block;
        public byte ViPen2_Get_Wave_ID;

        public byte ViPen2_Data_Blocks;

        public uint Timestamp;
        public float Coeff;

        public uint DataType;
        public uint DataUnits;
        public uint DataLen;
        public float DataDX;

        public int SpectrumAvg;
        public int SpectrumAvgMax;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 4)]
        public short[] Values;

        public byte Reading;
        public byte Empty1; // IsCharging;
        public byte Empty2; // Battery;
        public byte Align1;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = VIPEN2_BLUETOOTH_PACKET_DATA_SIZE - 12 * 4)]
        public byte[] Reserv2;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct stVPen2Data
    {
        public stVPen2Header Header;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = 128)]
        public stVPen2Block[] Blocks;
    }

    [StructLayout(LayoutKind.Sequential, Pack = 1)]
    public struct stVPen2Block
    {
        private const int VIPEN2_STAMPS_IN_BLOCK = ((stVPen2Header.VIPEN2_BLUETOOTH_PACKET_DATA_SIZE - 2) / 2);

        public byte ViPen_Get_Data_Block;
        public byte ViPen_Get_Wave_ID;

        [MarshalAs(UnmanagedType.ByValArray, SizeConst = VIPEN2_STAMPS_IN_BLOCK)]
        public short[] Data;
    }


}