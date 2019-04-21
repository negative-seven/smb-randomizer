using smb_randomizer.utils;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace smb_randomizer
{
	public class MemoryManager
	{
		private Process process;

		public MemoryManager(string processName)
		{
			var processes = Process.GetProcessesByName(processName);

			if (processes.Count() > 0)
			{
				process = processes[0];
			}
			else
			{
				throw new Exception("Process not found.");
			}
		}

		public IntPtr GetModuleBaseAddress(string moduleName)
		{
			foreach (ProcessModule module in process.Modules)
			{
				if (module.ModuleName == moduleName)
				{
					return module.BaseAddress;
				}
			}

			return IntPtr.Zero;
		}

		public IntPtr GetMainModuleBaseAddress()
		{
			return process.MainModule.BaseAddress;
		}
		
		public uint ReadUnsignedInt(IntPtr address)
		{
			var bytes = ReadBytes(address, sizeof(uint));
			return BitConverter.ToUInt32(bytes, 0);
		}

		public string ReadNullTerminatedString(IntPtr address, int maxLength)
		{
			var value = "";

			for (var index = 0; index < maxLength; index++)
			{
				var c = (char)ReadBytes(address + index, 1)[0];

				if (c == '\0')
				{
					break;
				}
				else
				{
					value += c;
				}
			}

			return value;
		}

		public byte[] ReadBytes(IntPtr address, int count)
		{
			var value = new byte[count];

			if (!WinAPI.ReadProcessMemory(process.Handle, address, value, count, out _))
			{
				throw new Exception("Could not read from memory.");
			}

			return value;
		}

		public void WriteUnsignedInteger(IntPtr address, UInt32 value)
		{
			WriteBytes(address, BitConverter.GetBytes(value));
		}

		public void WriteBytes(IntPtr address, byte[] data)
		{
			if (!WinAPI.WriteProcessMemory(process.Handle, address, data, data.Count(), out _))
			{
				throw new Exception("Could not write to memory.");
			}
		}
	}
}
