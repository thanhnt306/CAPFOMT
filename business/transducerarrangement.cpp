#include "transducerarrangement.h"

TransducerArrangement* TransducerArrangement::instance = nullptr;

void TransducerArrangement::initTransduccers(std::vector<Transducer *>* transducers)
{
    Transducer* trans1 = addTranducer();
    transducers->push_back(trans1);
    Transducer* trans2 = addTranducer(QVector3D(0.0f, 0.02f, 0.0f), QVector3D(0.0f, -1.0f, 0.0f));
    transducers->push_back(trans2);
}

Transducer *TransducerArrangement::addTranducer(QVector3D pos, QVector3D normal)
{
    return new Transducer(pos, normal);
}

TransducerArrangement::TransducerArrangement()
{

}
