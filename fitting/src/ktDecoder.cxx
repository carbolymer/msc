#include "ktDecoder.hxx"

Bool_t checkFile(const char *fileName)
{
	TFile* file = new TFile(fileName);
	if(file->IsZombie())
		return kFALSE;
	TH1D* cdenn1da = (TH1D*)  file->Get("cdenn1da");
	// if there is a very little amount of entries in first 1/5th of histogram - reject
	if(cdenn1da->Integral(1,TMath::Nint((double)cdenn1da->GetMaximumBin()/5))/cdenn1da->GetEntries() > 1e-5)
		return kTRUE;
	else
		return kFALSE;
}


// Bool_t mergeFiles(std::vector<ktBinFile> &inputFiles, ktBinFile &outputFile)
// {
// 	TFileMerger merger(kFALSE,kFALSE);
// 	merger.SetMsgPrefix("[merger]");
// 	merger.SetNotrees(kFALSE);

// 	if (!merger.OutputFile(boost::get<0>(outputFile).c_str(),kTRUE,1))
// 	{
// 		std::cerr << "Could not open output file: " << boost::get<0>(outputFile) << std::endl;
// 		return kFALSE;
// 	}
// 	for(std::vector<ktBinFile>::iterator it = inputFiles.begin() ; it != inputFiles.end() ; ++it)
// 		if(!merger.AddFile(boost::get<0>(*it).c_str()))
// 		{
// 			std::cerr << "could not add file: " << boost::get<0>(*it).c_str() << std::endl;
// 			return kFALSE;
// 		}
// 	boost::get<1>(outputFile) = boost::get<1>(inputFiles.front());
// 	boost::get<2>(outputFile) = boost::get<2>(inputFiles.back());
// 	return merger.Merge();
// }

Bool_t decodeKtBin(const unsigned int binNumber, double &ktMin, double &ktMax)
{
	switch(binNumber)
	{
		case 51:
			ktMin = 0.12;
			ktMax = 0.2;
			break;
		case 52:
			ktMin = 0.2;
			ktMax = 0.3;
			break;
		case 53:
			ktMin = 0.3;
			ktMax = 0.4;
			break;
		case 54:
			ktMin = 0.4;
			ktMax = 0.5;
			break;
		case 55:
			ktMin = 0.5;
			ktMax = 0.6;
			break;
		case 56:
			ktMin = 0.6;
			ktMax = 0.7;
			break;
		case 57:
			ktMin = 0.7;
			ktMax = 0.8;
			break;
		case 58:
			ktMin = 0.8;
			ktMax = 1.0;
			break;
		case 59:
			ktMin = 1.0;
			ktMax = 1.2;
			break;
		default:
			std::cerr << "\tUnknown kt bin: " << binNumber << std::endl;
			return kFALSE;
			break;
	}
	return kTRUE;
}

Bool_t parseFileName(const char *fileName, double &ktMin, double &ktMax)
{
	unsigned int ktBin;
	ktMin = 0;
	ktMax = 0;
	boost::cmatch matches;
	boost::regex pattern("(.*?)(\\d+)(.*?)");
	boost::regex_match(basename((char*)fileName), matches, pattern);
	ktBin = boost::lexical_cast<unsigned long int>(matches[2]);
	return decodeKtBin(ktBin, ktMin, ktMax);
}

Bool_t loadFileList(std::istream &inputFilesList, std::vector<ktBinFile> &inputFiles)
{
	char buffer[256];
	static int counter = 0;
	double ktBinMin, ktBinMax;
  	while (inputFilesList.good())
    {
    	std::fill(buffer, buffer+255, 0);
    	inputFilesList >> buffer;
    	if(buffer[0] == '\0')
    		continue;
   		std::cerr << buffer;
			if(checkFile(buffer))
    	{
				std::cerr << "\t\t[ OK ]" << std::endl;
				if(parseFileName(buffer, ktBinMin, ktBinMax))
					inputFiles.push_back(boost::make_tuple(std::string(buffer), ktBinMin, ktBinMax));
			}
			else
			{
				std::cerr << "\t\t[ !BAD! ]" << std::endl;
			}
		}
    return kTRUE;
}


int main(int argc, char **argv)
{
	std::vector<ktBinFile> files;
	if(argc < 1)
	{	std::cerr << argv[0] << " " << "pp" << std::endl <<
				"Pass to the argument pair identifier and file list to stdin" << std::endl;
		return 1;
	}
	if(loadFileList(std::cin, files))
	{
		for(int i = 0, size = files.size(); i < size; ++i)
			std::cout << boost::get<0>(files[i]) << " " << boost::get<1>(files[i]) << " " << boost::get<2>(files[i]) << std::endl;
		return 0;
	}
	else
		std::cerr << "Could not load file list" << std::endl;
	return 1;
}