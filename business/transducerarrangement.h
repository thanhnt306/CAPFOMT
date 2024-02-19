#ifndef TRANSDUCERARRANGEMENT_H
#define TRANSDUCERARRANGEMENT_H

#include <simmulation/transducer.h>

class TransducerArrangement
{
public:
    static TransducerArrangement* getInstance()
    {
        if(!instance)
        {
            instance = new TransducerArrangement();
        }
        return instance;
    };

    void initTransduccers(std::vector<Transducer *> *transducers);
    Transducer* addTranducer(Vec3D pos = Vec3D(0,0,0), Vec3D normal= Vec3D(0,1,0));
private:

    static TransducerArrangement* instance;
    TransducerArrangement();

};

#endif // TRANSDUCERARRANGEMENT_H
