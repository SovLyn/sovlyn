#pragma once
#ifndef SERIALIZABLE_HPP_OFHMG2UA
#define SERIALIZABLE_HPP_OFHMG2UA

namespace sovlyn{
namespace serialize{
class DataStream;
class Serialzable{
	public:
		virtual void serialize(DataStream & datastream) const =0;
		virtual void unserialize(DataStream & datastream) =0;
};
}
}

#define SERIALIZE(...)                                \
    void serialize(sovlyn::serialize::DataStream & stream) const override\
    {                                                 \
        char type = sovlyn::serialize::CUSTOM;        \
        stream.write((char *)&type, sizeof(char));    \
        stream.write_args(__VA_ARGS__);               \
    }                                                 \
                                                      \
    void unserialize(sovlyn::serialize::DataStream & stream) override    \
    {                                                 \
        stream.read_args(__VA_ARGS__);                \
    }

#endif /* end of include guard: SERIALIZABLE_HPP_OFHMG2UA */
