#ifndef _MULTIFITPLOT_CXX_
#define _MULTIFITPLOT_CXX_

#include <sstream>
#include <vector>
#include <utility>
#include <TF1.h>
#include "MultiPlot.cxx"

using namespace std;

typedef std::pair<Double_t,Double_t> DValWithE;

class MultiFitPlot: public MultiPlot
{
private:
	TF1 *_fittingFunction;
	TLatex _coefficients;
	double _alpha, _alphaE, _gamma, _gammaE;

protected:
	MultiFitPlot& _copyIntoMe(const MultiFitPlot &rhs)
	{
		_alpha = rhs._alpha;
		_gamma = rhs._gamma;
		_alphaE = rhs._alphaE;
		_gammaE = rhs._gammaE;
		_fittingFunction = new TF1(*rhs._fittingFunction);
		MultiPlot::_copyIntoMe(rhs);
		return *this;
	}

	bool isInArray(const vector<unsigned short> &array, const unsigned short &value)
	{
		for(unsigned int i = 0; i < array.size(); ++i)
			if(array[i] == value)
				return true;
		return false;
	}

public:

	enum DispersionPlotType
	{
		// y / fittedFunction(x)
		Division,
		// y - fittedFunction(x)
		Difference,
		// (y - fittedFunction(x))^2
		SquaredDifference
	};

	static TF1* defaultFunction;

	MultiFitPlot() :
		MultiPlot(""),
		_alpha(0),
		_alphaE(0),
		_gamma(0),
		_gammaE(0)
	{
		_fittingFunction = new TF1(*defaultFunction);
	}

	MultiFitPlot(const char _labels[]) :
		MultiPlot(_labels),
		_alpha(0),
		_alphaE(0),
		_gamma(0),
		_gammaE(0)
	{
		_fittingFunction = new TF1(*defaultFunction);
	}

	MultiFitPlot(const TF1* fittingFunction, const char _labels[] = "") :
		MultiPlot(_labels),
		_alpha(0),
		_alphaE(0),
		_gamma(0),
		_gammaE(0)
	{
		_fittingFunction = (TF1*) fittingFunction;
	}

	MultiFitPlot(const MultiFitPlot &rhs) : MultiPlot("")
	{
		_copyIntoMe(rhs);
	}

	MultiPlot& operator=(const MultiFitPlot &rhs)
	{
		return _copyIntoMe(rhs);
	}

	void Fit()
	{
		Fit(vector<unsigned short>());
	}

	// Argument contains numbers of datasets which will be fitted
	// 
	void Fit(vector<unsigned short> graphFilterIndices)
	{
		Double_t *ey, *x, *y, it =0, nPoints = 0;
		for( unsigned int i = 0; i < graphCount; ++i)
			nPoints += graphs[i]->GetN();

		TGraphErrors *allPoints = new TGraphErrors();
		for( unsigned int i = 0; i < graphCount; ++i)
		{
			if(graphFilterIndices.size() != 0)
			{
				if(!isInArray(graphFilterIndices,i))
					continue;
			}
			nPoints = graphs[i]->GetN();
			x = graphs[i]->GetX();
			ey = graphs[i]->GetEY();
			y = graphs[i]->GetY();
			for(unsigned int j = 0; j < nPoints; ++j)
			{
				allPoints->SetPoint(it,x[j],y[j]);
				allPoints->SetPointError(it,0,ey[j]);
				++it;
			}
		}
		_fittingFunction->SetParameter(0,1);
		_fittingFunction->SetParameter(1,1);
		allPoints->Fit(_fittingFunction,"S");
		// custom
		_alpha = _fittingFunction->GetParameter(0);
		_gamma = _fittingFunction->GetParameter(1);
		_alphaE = _fittingFunction->GetParError(0);
		_gammaE = _fittingFunction->GetParError(1);
	}

	void Draw()
	{
		MultiPlot::Draw();
		_fittingFunction->Draw("SAME");
		_fittingFunction->SetLineColor(1);
		_fittingFunction->SetLineWidth(2);

		std::stringstream a, aE, g, gE;
		a.precision(3);
		aE.precision(3);
		g.precision(3);
		gE.precision(3);

		a << _alpha;
		aE << _alphaE;
		g << _gamma;
		gE << _gammaE;

		std::string pm = std::string(" #pm ");

		if(_alpha != 0)
		{
			_coefficients.Draw();
			if(yMax > 11)
				_coefficients.DrawLatex(0.33, 10.8, 
					(std::string("#splitline{#alpha = ") + a.str() + pm + aE.str() 
					+ std::string("}{#gamma = ") + g.str() + pm + gE.str() + std::string("}")).c_str() );
			else if(yMax > 7)
				_coefficients.DrawLatex(0.33, 8.8, 
					(std::string("#splitline{#alpha = ") + a.str() + pm + aE.str() 
					+ std::string("}{#gamma = ") + g.str() + pm + gE.str() + std::string("}")).c_str() );
			else
				_coefficients.DrawLatex(0.33, 6, 
					(std::string("#splitline{#alpha = ") + a.str() + pm + aE.str() 
					+ std::string("}{#gamma = ") + g.str() + pm + gE.str() + std::string("}")).c_str() );
		}
	}

private:
	DValWithE CalculateDispersionFactor(DispersionPlotType type, Double_t x, Double_t y, Double_t yError)
	{
		DValWithE result;
		switch(type)
		{
			case Difference :
				result.first = y - _fittingFunction->Eval(x);
				result.second = yError;
				break;
			case Division :
				result.first = y/_fittingFunction->Eval(x);
				result.second = yError/_fittingFunction->Eval(x);
				break;
			case SquaredDifference :
				result.first = (y - _fittingFunction->Eval(x))*(y - _fittingFunction->Eval(x));
				result.second = 2.0*(y - _fittingFunction->Eval(x))*yError;
				break;				
		}
		return result;
	}

	pair<Double_t,Double_t> GetPlotYRange(DispersionPlotType type)
	{
		pair<Double_t,Double_t> yRange;
		switch(type)
		{
			case Difference :
				yRange.first = -1;
				yRange.second = 1;
				break;
			case Division :
				yRange.first = 0.7;
				yRange.second = 1.4;
				break;
			case SquaredDifference :
				yRange.first = -0.1;
				yRange.second = 0.5;
				break;
		}
		return yRange;
	}

public:

	MultiPlot& GetDispersionPlot(DispersionPlotType type)
	{
		MultiPlot *mp = new MultiPlot();
		unsigned int i, j, nPoints;
		DValWithE point;
		pair<Double_t,Double_t> yRange = GetPlotYRange(type);

		Double_t *yErrors;
		Double_t x,y;
		mp->labels = labels;
		mp->yMin = yRange.first;
		mp->yMax = yRange.second;
		mp->legendY1 = 0.95;
		for(j=0 ; j < graphCount; ++j )
		{
			mp->theme[j] = theme[j];
			mp->graphNames[j] = graphNames[j];
		}
		for(i = 0; i < graphCount; ++i)
		{
			nPoints = graphs[i]->GetN();
			yErrors = graphs[i]->GetEY();
			for(j = 0; j < nPoints; ++j)
			{
				graphs[i]->GetPoint(j, x, y);
				point = CalculateDispersionFactor(type, x, y, yErrors[i]);
				mp->graphs[i]->SetPoint(j, x, point.first);
				mp->graphs[i]->SetPointError(j, 0, point.second);
			}
		}
		return *mp;
	}
};

TF1*  MultiFitPlot::defaultFunction = 0;

#endif