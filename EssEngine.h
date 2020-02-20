#pragma once
#include <vector>

namespace ESS
{
	using Signal = std::vector<float>;

class EssEngine {
public:
	void SetSignalParameters(int InSampleRate, int InOctavesCount, int InLengthCoefficient);

	void GenerateReferenceSignal();
	void ImportProcessedSignal(const Signal& InProcessedSignal);

	void CalculateRawIR();
	void SeparateIRs(int HarmonicsCount, double IRMaxLength, double HarmonicLength, double PreFrame);

	Signal GetReferenceSignal() const;
	Signal GetRawIR() const;
	std::vector<Signal> GetIRArray() const;

private:
	Signal GenerateInverseSignal(const Signal& RefSignal);
	void FadeReferenceSignal(double WidthInOctaves);
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
