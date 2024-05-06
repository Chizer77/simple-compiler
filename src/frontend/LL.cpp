#include "frontend/LL.h"



CFG* LL::LeftRecurElimination(const CFG& cfg) {
    CFG *opg = new CFG();
    return opg;
}



CFG* LL::LeftFactorExtraction(const CFG& cfg) {
     CFG* opg = new CFG();

     opg->start = cfg.start;
     opg->nonter = cfg.nonter;
     opg->ter = cfg.ter;


        bool factor_something = true;

        while(factor_something)

          for (int nonter : cfg.nonter) {

              std::vector<int> factor;
              std::vector<std::vector<int>> factored_productions;
              std::vector<std::vector<int>> factoring_productions;

               for (const auto& prod1 : cfg.products){
                    if(prod1.start == nonter){
                       factoring_productions.push_back(prod1.grammar);
                    }
               }

             bool factor_something = true;
                    while(factor_something){
                        int mf = 0;
                        for (const auto& prod1 : factoring_productions){
                                for(const auto& prod2 : factoring_productions){
                                    if(prod1.grammar != prod2.grammar){
                                          int minLen = std::min(prod1.size(), prod2.size());

                                          for (int i = 0; i < minLen; i++) {
                                              if (prod1[i] != prod2[i]) {
                                                  break;
                                              }
                                          }

                                          if (i > mf) {
                                            mf = i ;
                                             for (int i = 0; i < minLen; i++) {
                                                  factor.push_back(prod1[i]);
                                             }
                                          }

                                      }
                                    }
                            }

                            for (const auto& prod :factoring_productions){
                                for (int i = 0; i < mf ; i++){
                                    if (prod[i] != factor[i]) {
                                        factored_productions.push_back(prod);
                                        break;
                                    }
                                }
                                if (i == mf){
                                    int newId = cfg.newID();
                                    int Empty = CFG::EMPTY_ID;

                                    std::vector<int> new_production;
                                    new_production.push_back(factor);
                                    new_production.push_back(newId);
                                    opg->products.insert(Productions(nonter, new_production));
                                    //这个因子刚好就是一整个产生式
                                    if (mf == prod.size()){
                                       opg->products.insert(Productions(newId, Empty));
                                    }else{
                                        new_production.clear();
                                       new_production.insert(new_production.end(), prod.begin() + mf, prod.end());
                                    }
                                }
                            }
                            std::swap(factored_productions,factoring_productions);
                            factored_productions.clear();

                            //剩下无法提取左因子的部分
                            if (mf == 0){
                                factor_something = false;
                                 for (const auto& prod :factoring_productions){
                                    opg->products.insert(Productions(nonter, prod));
                                 }
                            }

                        }
                    }

          return opg;
}