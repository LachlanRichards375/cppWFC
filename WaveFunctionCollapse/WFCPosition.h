#pragma once
#include <optional>
struct WFCPosition
{
public:
	int x;
	int y;
	std::optional<int> z;
	std::optional<int> w;

	WFCPosition(WFCPosition& other)
	{
		WFCPosition::x = other.x;
		WFCPosition::y = other.y;
		WFCPosition::z = other.z;
		WFCPosition::w = other.w;
	}

	WFCPosition(WFCPosition* other)
	{
		WFCPosition::x = other->x;
		WFCPosition::y = other->y;
		WFCPosition::z = other->z;
		WFCPosition::w = other->w;
	}

	WFCPosition(int x, int y)
	{
		WFCPosition::x = x;
		WFCPosition::y = y;
	}

	WFCPosition(int x, int y, int z)
	{
		WFCPosition::x = x;
		WFCPosition::y = y;
		WFCPosition::z = z;
	}

	WFCPosition(int x, int y, int z, int w)
	{
		WFCPosition::x = x;
		WFCPosition::y = y;
		WFCPosition::z = z;
		WFCPosition::w = w;
	}

	bool operator==(const WFCPosition& other) const
	{
		return (
			this->x == other.x &&
			this->y == other.y &&
			this->z == other.z &&
			this->w == other.w);
	}

	bool operator!=(const WFCPosition& other) const
	{
		return !(
			this->x == other.x &&
			this->y == other.y &&
			this->z == other.z &&
			this->w == other.w);
	}

	WFCPosition operator-(const WFCPosition& other) const {
		if (other.w.has_value()) {
			return WFCPosition(
				this->x - other.x,
				this->y - other.y,
				this->z.value() - other.z.value(),
				this->w.value() - other.w.value()
			);
		}

		if (other.z.has_value()) {
			return WFCPosition(
				this->x - other.x,
				this->y - other.y,
				this->z.value() - other.z.value()
			);
		}

		return WFCPosition(
			this->x - other.x,
			this->y - other.y
		);
	}
};
