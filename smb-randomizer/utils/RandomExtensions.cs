using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace smb_randomizer.utils
{
	public static class RandomExtensions
	{
		public static void ShuffleInPlace<T>(this Random random, ref IList<T> collection)
		{
			for (var firstSwapIndex = collection.Count() - 1; firstSwapIndex > 0; firstSwapIndex--)
			{
				var secondSwapIndex = random.Next(firstSwapIndex + 1);
				T temp = collection[firstSwapIndex];
				collection[firstSwapIndex] = collection[secondSwapIndex];
				collection[secondSwapIndex] = temp;
			}
		}
	}
}
