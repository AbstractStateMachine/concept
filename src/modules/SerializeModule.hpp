/*
** Test cases for showing almost all aspects of serializing and deserializing DataObjects.
** The following code samples show the optional interface for
** ser- and deserialization of the content of a DataObject
*/

#include "../asm/asm.hpp"
#include "../datatypes/MyComplexDOType.hpp"

class SerializeModule {
  private:
    // Private DOs are ignored by the map_maker tool and will not be serialized by the system
    Asm::DataObject<std::list<double>> doPrivate;

    // Example for a serialization function located in a module
    void testSerFn4doFunc(rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
        doString3.get([&value, &allocator](std::string s) {
            value.SetString(s.c_str(), allocator);
        });
    }

    // Example for a deserialization function located in a module
    void testDeSerFn4doFunc(rapidjson::Value& value) {
        doString3.set([&value](std::string& s) {
            s = value.GetString();
        });
    }

  protected:
    // Protected DOs are ignored by the map_maker tool and will not be serialized by the system
    Asm::DataObject<std::map<std::string, double>> doProtected;

  public:
    Asm::DataObject<int> doInt;
    Asm::DataObject<double> doDouble;
    Asm::DataObject<std::string> doString;
    Asm::DataObject<bool> doBool;
    Asm::DataObject<std::string> doString2;
    Asm::DataObject<std::string> doString3;
    Asm::DataObject<MyComplexDOType> doMyComplexDOType;
    Asm::DataObject<unsigned int> doUnsignedInt{42, Asm::default_serializer};
    Asm::DataObject<int64_t> doInt64{-8000000000, Asm::default_serializer};
    Asm::DataObject<uint64_t> doUInt64{8000000000, Asm::default_serializer};

    SerializeModule() :
        // No serialization at all if only one parameter is used for the content
        doPrivate(std::list<double> {
        {
            1.1, 2.2, 3.3
        }
    }),
    doProtected(std::map<std::string, double> { {"42", 22.0}, { "43", 23.0 }, { "44", 24.0 }}),
    // Only for primitive datatypes the build in serialization of the DataObject can be activated by a boolean value as second parameter of the DataObject constructor
    // It does not matter if it is true or false; this is only a dummy value for selecting this constructor
    // Please use "Asm::default_serializer" for a better semantic understanding
    doInt(3, Asm::default_serializer),
    doDouble(1.1, true),
    doString("0-1", false),
    // You can set your own de-/serialization function as lambda expression (not highly recommended for complex datatypes)
    doBool(false, [&](rapidjson::Value& value, rapidjson::Document::AllocatorType&) {
        doBool.get([&value](bool b) {
            value.SetBool(b);
        });
    }, [&](rapidjson::Value& value) {
        doBool.set([&value](std::atomic<bool>& b) {
            b = value.GetBool();
        });
    }),
    doString2("dummy", [&](rapidjson::Value& value, rapidjson::Document::AllocatorType& allocator) {
        doString2.get([&](std::string s) {
            value.SetString(s.c_str(), allocator);
        });
    }, [&](rapidjson::Value& value) {
        doString2.set([&value](std::string s) {
            s = value.GetString();
        });
    }),
    // Simple function pointers
    doString3("test", &SerializeModule::testSerFn4doFunc, &SerializeModule::testDeSerFn4doFunc, this),
    // Function pointer on the content
    // Boolean is again a dummy value for selecting the right constructor
    // This is the prefered way to do serialization on complex types
    // Let the DataObject serialize it self, it has the best knowledge what do
    doMyComplexDOType(MyComplexDOType{ 1, 2, "12" }, &MyComplexDOType::serializeMe, &MyComplexDOType::deserializeMe) {}
};
