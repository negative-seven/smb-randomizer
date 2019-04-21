using smb_randomizer.utils;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace smb_randomizer
{
	public class GameRandomizer
	{
		private MemoryManager memoryManager;

		public GameRandomizer()
		{
			memoryManager = new MemoryManager("SuperMeatBoy");
		}

		public bool DisableLeaderboards()
		{
			/*
			Replace code at SuperMeatBoy.exe+0x120D6:

			jne SuperMeatBoy.exe+0x12139

			with:

			nop
			nop
			*/

			var oldBytes = new byte[] { 0x75, 0x61 };
			var newBytes = new byte[] { 0x90, 0x90 };

			return PatchBytes(memoryManager.GetMainModuleBaseAddress() + 0x120D6, oldBytes, newBytes);
		}

		public void RandomizeLevels(int seed)
		{
			IList<IntPtr> fileNamePointers = new List<IntPtr>();
			IList<int> fileNameIndices = new List<int>();

			var levelFileNames = GameLevel.GetAll().Select(level => level.FileName);

			var baseAddress = memoryManager.GetMainModuleBaseAddress();
			var fileListPointer = (IntPtr)memoryManager.ReadUnsignedInt(baseAddress + 0x2da120);
			var fileListSize = memoryManager.ReadUnsignedInt(fileListPointer + 0xc);
			var fileListStartPointer = (IntPtr)memoryManager.ReadUnsignedInt(fileListPointer + 0x18);

			// Find all matching level name pointers
			for (var elementIndex = 0; elementIndex < fileListSize; elementIndex++)
			{
				var fileNamePointer = (IntPtr)memoryManager.ReadUnsignedInt(fileListStartPointer + elementIndex * sizeof(UInt32));
				var fileName = memoryManager.ReadNullTerminatedString(fileNamePointer, 256);

				if (levelFileNames.Contains(fileName))
				{
					fileNameIndices.Add(elementIndex);
					fileNamePointers.Add(fileNamePointer);
				}
			}

			// Write shuffled level name pointers
			new Random(seed).ShuffleInPlace(ref fileNamePointers);
			for (int i = 0; i < fileNamePointers.Count; i++)
			{
				memoryManager.WriteUnsignedInteger(fileListStartPointer + fileNameIndices[i] * sizeof(UInt32), (uint)fileNamePointers[i]);
			}
		}

		private bool PatchBytes(IntPtr address, byte[] oldBytes, byte[] newBytes)
		{
			if (oldBytes.Length != newBytes.Length)
			{
				throw new ArgumentException("The bytes parameters are of different sizes.");
			}

			var readBytes = memoryManager.ReadBytes(address, oldBytes.Count());
			if (readBytes == oldBytes)
			{
				memoryManager.WriteBytes(address, newBytes);
			}
			else if (readBytes != newBytes)
			{
				return false;
			}

			return true;
		}
	}
}
