using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace smb_randomizer
{
	public class GameLevel
	{
		public string InternalName { get; private set; }
		public string FileName
		{
			get
			{
				return String.Format("Levels/{0}.smbl", InternalName);
			}
		}

		private GameLevel(string internalName)
		{
			this.InternalName = internalName;
		}

		public static IEnumerable<GameLevel> GetAll()
		{
			List<string> levelNames = new List<string>();

			// Worlds 1-5
			for (int world = 1; world <= 5; world++)
			{
				for (int level = 1; level <= 20; level++)
				{
					levelNames.Add(String.Format("{0}-{1}", world, level));
				}
			}

			// World 6
			for (int level = 1; level <= 5; level++)
			{
				levelNames.Add(String.Format("6-{0}", level));
			}

			// World 7
			for (int level = 1; level <= 20; level++)
			{
				levelNames.Add(String.Format("bonus1-{0}", level));
			}

			// Dark world levels of worlds 1-7
			levelNames.AddRange(levelNames.ToList().Select(name => name + "x"));

			// Warp zone levels
			var warpZonePrefixes = new string[] { "hretro", "retro1-", "retro2-", "retro3-", "retro4-", "retro5-", "retro6-", "retro7-", "4bit2-", "4bit3-", "4bit4-", "gameboy", "gameboy2-", "gameboy3-", "gameboy4-" };
			foreach (var prefix in warpZonePrefixes)
			{
				for (int level = 1; level <= 3; level++)
				{
					levelNames.Add(prefix + level.ToString());
				}
			}

			// Glitch levels
			for (int world = 1; world <= 6; world++)
			{
				levelNames.Add("-" + world.ToString());
			}

			return levelNames.Select(name => new GameLevel(name));
		}
	}
}
