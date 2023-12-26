using System.Numerics;
using System;

namespace c_Tests
{
	public struct WFCPosition
	{
		public float x;
		public float y;
		public float? z;
		public float? w;

		public WFCPosition(WFCPosition other)
		{
			this.x = other.x;
			this.y = other.y;
			this.z = other.z;
			this.w = other.w;
		}

		public WFCPosition(float x, float y)
		{
			this.x = x;
			this.y = y;
			this.z = null;
			this.w = null;
		}

		public WFCPosition(float x, float y, float z)
		{
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = null;
		}

		public WFCPosition(float x, float y, float z, float w)
		{
			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}

		public bool IsVector3()
		{
			return !(z is null);
		}

		public bool IsVector4()
		{
			return !(z is null && w is null);
		}
		public override string ToString()
		{
			string s = $"({x},{y}";
			if (IsVector3())
			{
				s += $",{z}";
			}
			if (IsVector4())
			{
				s += $",{w}";
			}
			return s + ")";
		}

		public static bool operator ==(WFCPosition pos1, WFCPosition pos2)
		{
			return (
				pos1.x == pos2.x &&
				pos1.y == pos2.y &&
				pos1.z == pos2.z &&
				pos1.w == pos2.w
			);
		}

		public static bool operator !=(WFCPosition pos1, WFCPosition pos2)
		{
			return !(
				pos1.x == pos2.x &&
				pos1.y == pos2.y &&
				pos1.z == pos2.z &&
				pos1.w == pos2.w
			);
		}

		public override bool Equals(object obj)
		{
			if (obj == null || !this.GetType().Equals(obj.GetType()))
			{
				return false;
			}
			else
			{
				WFCPosition other = (WFCPosition)obj;
				return this == other;
			}
		}

		public override int GetHashCode()
		{
			return base.GetHashCode();
		}
	}
}