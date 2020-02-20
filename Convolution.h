#pragma once
#include <vector>

namespace ESS
{
	using Signal = std::vector<float>;
	void Convolution(const Signal& FirstSignal, const Signal& SecondSignal, Signal& OutputSignal);
}
