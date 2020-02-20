#pragma once

namespace ESS
{
	using Signal = std::vector<float>;

	bool SaveToFile(const std::string& FilePath, const Signal& SourceSignal, int BitDepth, int SampleRate, double Attenuator);
	Signal ImportFromFile(const std::string& FilePath);
}
