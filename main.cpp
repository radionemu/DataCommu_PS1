#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <vector>

using namespace std;

vector<vector<int>> binaryStreams; 

void upTransition(vector<double> &xsp, vector<double> &ysp, size_t t){
    ysp.push_back(-1.0);
    xsp.push_back(t/2.0);
    ysp.push_back(-1.0);
    xsp.push_back((t+1)/2.0);
    ysp.push_back(1.0);
    xsp.push_back((t+1)/2.0);
    ysp.push_back(1.0);
    xsp.push_back((t+2)/2.0);
}

void downTransition(vector<double> &xsp, vector<double> &ysp, size_t t){
    ysp.push_back(1.0);
    xsp.push_back(t/2.0);
    ysp.push_back(1.0);
    xsp.push_back((t+1)/2.0);
    ysp.push_back(-1.0);
    xsp.push_back((t+1)/2.0);
    ysp.push_back(-1.0);
    xsp.push_back((t+2)/2.0);
}

void DiffManchester(vector<int> &bistream, vector<double> &xsp, vector<double> &ysp){
    size_t t = 0; //t%2 == 0 clock edge
    bool inverse = false;//true = down, false = up
    for(int i=0; i<bistream.size(); i++){
        //clock edge
        if(bistream[i] == 1){
            inverse = !inverse;
        }
        if(inverse){
            downTransition(xsp, ysp, t);
        }else{
            upTransition(xsp, ysp, t);
        }
        t+=2;
    }
}

void Manchester(vector<int> &bistream, vector<double> &xsp, vector<double> &ysp){
    size_t t = 0; //t%2 == 0 clock edge
    for(int i=0; i<bistream.size(); i++){
        //clock edge
            if(bistream[i] == 0){
                upTransition(xsp, ysp, t);
            }else{
                downTransition(xsp, ysp, t);
            }
            t+=2;
    }
}

void Settings(StringReference *errorMessage, RGBABitmapImageReference *imageReference, ScatterPlotSeries *series, ScatterPlotSettings *settings, vector<double> *xs, vector<double> *ys){
    bool success;
    series->xs = xs;
    series->ys = ys;
    series->linearInterpolation = true;
    series->lineThickness = 2;
    series->color = CreateRGBColor(1,0,0);

    settings->width = 600;
    settings->height = 400;
    settings->title = toVector(L" ");
    settings->xLabel = toVector(L"Time");
    settings->yLabel = toVector(L"Pulse");
    settings->yMin = -2.0f;
    settings->yMax = 2.0f;
    settings->showGrid = true;
    settings->scatterPlotSeries->push_back(series);
}

void render(vector<double> &xs, vector<double> &ys, string str, int i){
    StringReference * errorMessage = CreateStringReferenceLengthValue(0, L' ');
    RGBABitmapImageReference * imageReference = CreateRGBABitmapImageReference();
    ScatterPlotSeries *series = GetDefaultScatterPlotSeriesSettings();
    ScatterPlotSettings *settings = GetDefaultScatterPlotSettings();
    Settings(errorMessage, imageReference, series, settings, &xs, &ys);
    bool success;
    success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);
    std::vector<double> *pngData = ConvertToPNG(imageReference->image);
    WriteToFile(pngData, (str+to_string(i)+".png"));
    DeleteImage(imageReference->image);
}

int main(){
    FILE * file = fopen("./Inputfile.txt", "r");
    if(file == NULL){
        printf("file open failed.\n");
        getchar();
        return 1;
    }
    printf("file open successed\n");
    binaryStreams.push_back(vector<int>());
    while(true){
        int ch = getc(file);
        //\n
        if(ch == EOF){
            break;
        }
        if(ch == '\n'){
            binaryStreams.push_back(vector<int>());
            continue;
        }
        int num = ch-'0';
        if(num == 0 || num == 1){
            binaryStreams.back().push_back(num);
        }else{
            printf("[Error] : Invalid Input Value\n");
            return 1;
        }
    }

    //for test
    for(int i=0; i<binaryStreams.size(); i++){
        vector<double> xs;
        vector<double> ys;
        Manchester(binaryStreams[i],xs, ys);
        render(xs, ys, "Manchester_line", i);
        printf("rendered Manchester line[%d]\n", i);
        xs.clear();
        ys.clear();
        DiffManchester(binaryStreams[i],xs, ys);
        render(xs, ys, "DifferentialManchester_line", i);
        printf("rendered Differential Manchester line[%d]\n", i);

        FreeAllocations();
    }


    return 0;
}