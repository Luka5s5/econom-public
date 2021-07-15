
struct Plot {
    double S_k;
    double S_b;
    double D_k;
    double D_b;
    // to count equilibrium P and Q
};

struct City {
    int id;
    std::vector<Plot> goods; 
    std::vector<int> army;  
    std::functional accept;
};
