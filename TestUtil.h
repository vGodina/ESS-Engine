#pragma once
#include <vector>

namespace ESS
{
	using Signal = std::vector<float>;

	struct HDLevels
	{
		double HD2;
		double HD3;
		double HD4;
		double HD5;
		double HD6;
		double HD7;
	};

	void AddNonlinearityToProcessedSignal(HDLevels HD, Signal& ProcessedSignal)
	{
		for (auto& Sample : ProcessedSignal)
		{
			Sample += pow(Sample, 2.0) * HD.HD2
				+ pow(Sample, 3.0) * HD.HD3
				+ pow(Sample, 4.0) * HD.HD4
				+ pow(Sample, 5.0) * HD.HD5
				+ pow(Sample, 6.0) * HD.HD6
				+ pow(Sample, 7.0) * HD.HD7;
		}
	}
}
