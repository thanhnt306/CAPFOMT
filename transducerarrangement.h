#ifndef TRANSDUCERARRANGEMENT_H
#define TRANSDUCERARRANGEMENT_H

#include <Transducer.h>

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
    Transducer* addTranducer(QVector3D pos = QVector3D(0,0,0), QVector3D normal= QVector3D(0,1,0));
private:

    static TransducerArrangement* instance;
    TransducerArrangement();

};

#endif // TRANSDUCERARRANGEMENT_H
