#pragma once
#include <vector>

namespace ESS
{
	using Signal = std::vector<float>;

class EssEngine {
public:
	int EstimateLengthCoeff(double LengthInSec);
	int EstimateOctaveCount(double LowerFrequency); //Upper frequency is half SampleRate

	void SetSampleRate(int InSampleRate);
	void SetOctavesCount(int InOctavesCount);
	void SetLengthCoefficient(int InLengthCoefficient);

	void SetKirkebyFilter();

	void GenerateReferenceSignal();
	void ImportProcessedSignal(const Signal& InProcessedSignal);

	void CalculateRawIR();
	void SeparateIRs(int HarmonicsCount, double IRMaxLength, double HarmonicLength, double PreFrame);

	Signal GetReferenceSignal() const;
	Signal GetRawIR() const;
	std::vector<Signal> GetIRArray() const;

private:
	Signal GenerateInverseSignal(const Signal& RefSignal);
	void TrimProcessedSignalToRef();

	int SampleRate;
	int OctavesCount;
	int LengthCoefficient;

	Signal ReferenceSignal;
	Signal InverseSignal;
	Signal ProcessedSignal;
	Signal RawIR;
	std::vector<Signal> IRVector;
	};
} // namespace ESS
