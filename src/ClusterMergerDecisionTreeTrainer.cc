#include "../interface/ClusterMergerDecisionTreeTrainer.h"

ClusterMergerDecisionTreeTrainer::ClusterMergerDecisionTreeTrainer(const std::string& inputPath, const std::string& outputPath)
{
	// double signalWeight     = 1.0;
	// double backgroundWeight = 1.0;
	// TFile* outputFile = TFile::Open(outputPath.c_str(), "RECREATE");
	// TMVA::Factory *factory = new TMVA::Factory("TMVAClassification", outputFile, 
	// 	"V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification"); 
	// TFile* inputFile = new TFile(inputPath.c_str());
	// if(!inputFile -> Is_Open())
	// {
	// 	std::cerr << "ClusterMergerDecisionTreeTrainer(): Error opening the input file." << std::endl;
	// }
	// TTree* sigTree = (TTree*)(inputFile -> Get("signalTree"));
	// if(!sigTree)
	// {
	// 	std::cerr << "ClusterMergerDecisionTreeTrainer(): Error opening the signal tree." << std::endl;
	// }
	// TTree* bgTree  = (TTree*)(inputFile -> Get("backgroundTree"));
	// if(!sigTree)
	// {
	// 	std::cerr << "ClusterMergerDecisionTreeTrainer(): Error opening the background tree." << std::endl;
	// }
	// factory -> AddSignalTree(   sigTree, signalWeight);
	// factory -> AddBackgroundTree(bgTree, backgroundWeight);
	// factory -> AddVariable("startPixel_1_X", 'I');
	// factory -> AddVariable("startPixel_1_Y", 'I');
	// factory -> AddVariable("endPixel_1_X", 'I');
	// factory -> AddVariable("endPixel_1_Y", 'I');
	// factory -> AddVariable("startPixel_2_X", 'I');
	// factory -> AddVariable("startPixel_2_Y", 'I');
	// factory -> AddVariable("endPixel_2_X", 'I');
	// factory -> AddVariable("endPixel_2_Y", 'I');
	// factory -> AddVariable("length_1", 'F');
	// factory -> AddVariable("length_2", 'F');
	// factory -> AddVariable("dir_1", 'F');
	// factory -> AddVariable("dir_2", 'F');
	// factory -> PrepareTrainingAndTestTree(mycuts, mycutb, "random"); 
	// factory -> BookMethod(TMVA::Types::kBDT, "BDT", "NTrees=400:MaxDepth=2");
	// factory -> TrainAllMethods();
	// factory -> TestAllMethods();
	// factory -> EvaluateAllMethods();
	// if(!gROOT->IsBatch()) TMVAGui( outfileName ); 
}