//
// Created by Michal_Marszalek on 10.10.2020.
//

#include "catch.hpp"
#include <Yaml/Node/Node.h>

TEST_CASE("Node Basic") {

    SECTION("default") {
        Node n;
        Node k;
        REQUIRE(n.Type() == NodeType::Null);
        REQUIRE(n != k);
        n = k;
        REQUIRE(n == k);
        n.clear();
        REQUIRE(n.Type() == NodeType::Null);
        REQUIRE(n == k);
        n.reset();
        REQUIRE(n != k);
    }

    SECTION("String") {
        Node n;
        n = "str";
        REQUIRE(n.Type() == NodeType::Scalar);
        REQUIRE(n.Scalar() == "str");
        REQUIRE(n.size() == 0);
    }

    SECTION("As") {
        Node n;
        n = true;
        REQUIRE(n.as<bool>());
        n = "str";
        REQUIRE(n == "str");
        REQUIRE(n.as<std::string>() == "str");
    }

    SECTION("Clear") {
        Node n;
        n = "str";
        n.reset();
        REQUIRE(n.Type() == NodeType::Null);
    }

    SECTION("Sequence") {
        Node n;

        n.push_back(5);
        n.push_back("key");
        n.push_back(9);

        REQUIRE(n.Type() == NodeType::Sequence);
        REQUIRE(n.size() == 3);

        n.clear();
        REQUIRE(n.size() == 0);
    }

    SECTION("Map") {
        Node n;

        n[5] = 5;
        n[4] = "key";
        n[3] = 9;

        REQUIRE(n.size() == 3);
        REQUIRE(n.Type() == NodeType::Map);
    }

    SECTION("Map from Sequence") {
        Node n;

        n[0] = 0;
        n[1] = 1;
        n[2] = 2;

        REQUIRE(n.Type() == NodeType::Sequence);

        n[4] = 4;

        REQUIRE(n.Type() == NodeType::Map);

        n.clear();

        n[0] = 0;
        n[1] = 1;
        n[2] = 2;
        n[3] = 2;

        REQUIRE(n.Type() == NodeType::Sequence);

        REQUIRE(n.remove(3));

        REQUIRE(n.Type() == NodeType::Sequence);

        REQUIRE(n.remove(1));

        REQUIRE(n.Type() == NodeType::Map);
    }

    SECTION("Circle") {
        Node n;

        n[0] = 5;
        n[1] = "key";
        n[2] = 9;
        n[n[0]] = 3;

        REQUIRE(n.Type() == NodeType::Map);
        REQUIRE(n.size() == 4);
        n[0] = 9;
        REQUIRE(n[n[0]] == 3);
    }

    SECTION("Iterator") {
        Node n;

        n[0] = 5;
        n[1] = "key";
        n[2] = 9;
        n[n[0]] = 3;

        int i = 0;
        for (auto &it: n) {
            if (i == 0) {
                REQUIRE(it.first == 0);
                REQUIRE(it.second == 5);
            }
            if (i == 1) {
                REQUIRE(it.first == 1);
                REQUIRE(it.second == "key");
            }
            if (i == 2) {
                REQUIRE(it.first == 2);
                REQUIRE(it.second == 9);
            }
            if (i == 3) {
                REQUIRE(it.first == n[0]);
                REQUIRE(it.second == 3);
            }
            ++i;
        }

        i = 0;
        const Node c = n;
        for (auto &it: c) {
            if (i == 0) {
                REQUIRE(it.first == 0);
                REQUIRE(it.second == 5);
            }
            if (i == 1) {
                REQUIRE(it.first == 1);
                REQUIRE(it.second == "key");
            }
            if (i == 2) {
                REQUIRE(it.first == 2);
                REQUIRE(it.second == 9);
            }
            if (i == 3) {
                REQUIRE(it.first == n[0]);
                REQUIRE(it.second == 3);
            }
            ++i;
        }
    }

    SECTION("Throw") {
        Node n;
        const Node &c = n;

        n[0] = 5;
        n[1] = "key";
        n[2] = 9;
        n[n[0]] = 3;

        REQUIRE_THROWS(c[6]);
        REQUIRE_NOTHROW(c.Scalar());
        REQUIRE_NOTHROW(n = "str");
        REQUIRE_THROWS(c[0]);
    }

        ///TESTS FROM YAML-CPP
    SECTION("NodeTest, SimpleScalar") {
        Node node = Node("Hello, World!");
        REQUIRE(node.IsScalar());
        REQUIRE("Hello, World!" == node.as<std::string>());
    }

    SECTION("NodeTest, IntScalar") {
        Node node = Node(15);
        REQUIRE(node.IsScalar());
        REQUIRE(15 == node.as<int>());
    }

    SECTION("NodeTest, SimpleAppendSequence") {
        Node node;
        node.push_back(10);
        node.push_back("foo");
        node.push_back("monkey");
        REQUIRE(node.IsSequence());
        REQUIRE(3 == node.size());
        REQUIRE(10 == node[0].as<int>());
        REQUIRE("foo" == node[1].as<std::string>());
        REQUIRE("monkey" == node[2].as<std::string>());
        REQUIRE(node.IsSequence());
    }
/*
    SECTION("NodeTest, SequenceElementRemoval") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        node.remove(1);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("a" == node[0].as<std::string>());
        REQUIRE("c" == node[1].as<std::string>());
    }

    SECTION("NodeTest, SequenceElementRemovalSizeCheck") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        REQUIRE(3 == node.size());
        node.remove(1);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("a" == node[0].as<std::string>());
        REQUIRE("c" == node[1].as<std::string>());
    }

    SECTION("NodeTest, SequenceFirstElementRemoval") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        node.remove(0);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("b" == node[0].as<std::string>());
        REQUIRE("c" == node[1].as<std::string>());
    }

    SECTION("NodeTest, SequenceFirstElementRemovalSizeCheck") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        REQUIRE(3 == node.size());
        node.remove(0);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("b" == node[0].as<std::string>());
        REQUIRE("c" == node[1].as<std::string>());
    }*/

    SECTION("NodeTest, SequenceLastElementRemoval") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        node.remove(2);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("a" == node[0].as<std::string>());
        REQUIRE("b" == node[1].as<std::string>());
    }

    SECTION("NodeTest, SequenceLastElementRemovalSizeCheck") {
        Node node;
        node[0] = "a";
        node[1] = "b";
        node[2] = "c";
        REQUIRE(3 == node.size());
        node.remove(2);
        REQUIRE(node.IsSequence());
        REQUIRE(2 == node.size());
        REQUIRE("a" == node[0].as<std::string>());
        REQUIRE("b" == node[1].as<std::string>());
    }

    SECTION("NodeTest, NodeAssignment") {
        Node node1;
        Node node2;
        node1[1] = 1;
        node1[2] = 2;
        node1[3] = 3;
        node2 = node1;
        REQUIRE(node1 == node2);
        REQUIRE(node1[1] == node2[1]);
        REQUIRE(node1[2] == node2[2]);
        REQUIRE(node1[3] == node2[3]);
    }

    SECTION("NodeTest, MapElementRemoval") {
        Node node;
        node["foo"] = "bar";
        node.remove("foo");
        ///REQUIRE(!node["foo"]);
    }

    SECTION("NodeTest, MapIntegerElementRemoval") {
        Node node;
        node[1] = "hello";
        node[2] = 'c';
        node["foo"] = "bar";
        REQUIRE(node.IsMap());
        node.remove(1);
        REQUIRE(node.IsMap());
    }

    SECTION("NodeTest, SimpleAssignSequence") {
        Node node;
        node[0] = 10;
        node[1] = "foo";
        node[2] = "monkey";
        REQUIRE(node.IsSequence());
        REQUIRE(3 == node.size());
        REQUIRE(10 == node[0].as<int>());
        REQUIRE("foo" == node[1].as<std::string>());
        REQUIRE("monkey" == node[2].as<std::string>());
        REQUIRE(node.IsSequence());
    }

    SECTION("NodeTest, SimpleMap") {
        Node node;
        node["key"] = "value";
        REQUIRE(node.IsMap());
        REQUIRE("value" == node["key"].as<std::string>());
        REQUIRE(1 == node.size());
    }
/*
    SECTION("NodeTest, MapWithUndefinedValues") {
        Node node;
        node["key"] = "value";
        node["undefined"];
        REQUIRE(node.IsMap());
        REQUIRE("value" == node["key"].as<std::string>());
        REQUIRE(1 == node.size());

        node["undefined"] = "monkey";
        REQUIRE("monkey" == node["undefined"].as<std::string>());
        REQUIRE(2 == node.size());
    }

    SECTION("NodeTest, SeqIntoMap") {
        Node node;
        node[0] = "test";
        node[1];
        node[2] = "value";
        REQUIRE(node.IsMap());
        REQUIRE("test" == node[0].as<std::string>());
        REQUIRE("value" == node[2].as<std::string>());
        REQUIRE(2 == node.size());
    }*/

    SECTION("NodeTest, RemoveUnassignedNode") {
        Node node;
        node["key"];
        node.remove("key");
        REQUIRE(0 == node.size());
    }

    SECTION("NodeTest, RemoveUnassignedNodeFromMap") {
        Node node;
        Node n;
        node[n];
        node.remove(n);
        REQUIRE(0 == node.size());
    }

    SECTION("NodeTest, MapForceInsert") {
        Node node;
        Node k1("k1");
        Node k2("k2");
        Node v1("v1");
        Node v2("v2");
        node[k1] = v1;
        node[k2] = v1;
        REQUIRE(node.IsMap());
        REQUIRE("v1" == node["k1"].as<std::string>());
        REQUIRE("v1" == node["k2"].as<std::string>());
        REQUIRE(2 == node.size());

        node.force_insert(k2, v2);
        REQUIRE("v1" == node["k1"].as<std::string>());
        REQUIRE("v1" == node["k2"].as<std::string>());
        REQUIRE(3 == node.size());
    }
/*
    SECTION("NodeTest, UndefinedConstNodeWithFallback") {
        Node node;
        const Node &cn = node;
        REQUIRE(cn["undefined"].as<int>(3) == 3);
    }

    SECTION("NodeTest, MapIteratorWithUndefinedValues") {
        Node node;
        node["key"] = "value";
        node["undefined"];

        std::size_t count = 0;
        for (auto it = node.begin(); it != node.end(); ++it)
            count++;
        REQUIRE(1 == count);
    }

    SECTION("NodeTest, ConstIteratorOnConstUndefinedNode") {
        Node node;
        const Node &cn = node;
        const Node &undefinedCn = cn["undefined"];

        std::size_t count = 0;
        for (auto it = undefinedCn.begin(); it != undefinedCn.end(); ++it) {
            count++;
        }
        REQUIRE(0 == count);
    }

    SECTION("NodeTest, IteratorOnConstUndefinedNode") {
        Node node;
        const Node &cn = node;
        const Node &undefinedCn = cn["undefined"];

        Node &nonConstUndefinedNode = const_cast<Node &>(undefinedCn);

        std::size_t count = 0;
        for (auto it = nonConstUndefinedNode.begin();
             it != nonConstUndefinedNode.end(); ++it) {
            count++;
        }
        REQUIRE(0 == count);
    }*/

    SECTION("NodeTest, SimpleSubkeys") {
        Node node;
        node["device"]["udid"] = "12345";
        node["device"]["name"] = "iPhone";
        node["device"]["os"] = "4.0";
        node["username"] = "monkey";
        REQUIRE("12345" == node["device"]["udid"].as<std::string>());
        REQUIRE("iPhone" == node["device"]["name"].as<std::string>());
        REQUIRE("4.0" == node["device"]["os"].as<std::string>());
        REQUIRE("monkey" == node["username"].as<std::string>());
    }
/**
    SECTION("NodeTest, StdArray") {
        std::array<int, 5> evens{{2, 4, 6, 8, 10}};
        Node node;
        node["evens"] = evens;
        std::array<int, 5> actualEvens = node["evens"].as<std::array<int, 5>>();
        REQUIRE(evens == actualEvens);
    }

    SECTION("NodeTest, StdVector") {
        std::vector<int> primes;
        primes.push_back(2);
        primes.push_back(3);
        primes.push_back(5);
        primes.push_back(7);
        primes.push_back(11);
        primes.push_back(13);

        Node node;
        node["primes"] = primes;
        REQUIRE(primes == node["primes"].as<std::vector<int>>());
    }

    SECTION("NodeTest, StdList") {
        std::list<int> primes;
        primes.push_back(2);
        primes.push_back(3);
        primes.push_back(5);
        primes.push_back(7);
        primes.push_back(11);
        primes.push_back(13);

        Node node;
        node["primes"] = primes;
        REQUIRE(primes == node["primes"].as<std::list<int>>());
    }

    SECTION("NodeTest, StdMap") {
        std::map<int, int> squares;
        squares[0] = 0;
        squares[1] = 1;
        squares[2] = 4;
        squares[3] = 9;
        squares[4] = 16;

        Node node;
        node["squares"] = squares;
        std::map<int, int> actualSquares = node["squares"].as<std::map<int, int>>();
        REQUIRE(squares == actualSquares);
    }

    SECTION("NodeTest, StdPair") {
        std::pair<int, std::string> p;
        p.first = 5;
        p.second = "five";

        Node node;
        node["pair"] = p;
        std::pair<int, std::string> actualP =
                node["pair"].as<std::pair<int, std::string>>();
        REQUIRE(p == actualP);
    }
*/
    SECTION("NodeTest, SimpleAlias") {
        Node node;
        node["foo"] = "value";
        node["bar"] = node["foo"];
        REQUIRE("value" == node["foo"].as<std::string>());
        REQUIRE("value" == node["bar"].as<std::string>());
        REQUIRE(node["bar"] == node["foo"]);
        REQUIRE(2 == node.size());
    }

    SECTION("NodeTest, AliasAsKey") {
        Node node;
        node["foo"] = "value";
        Node value = node["foo"];
        node[value] = "foo";
        REQUIRE("value" == node["foo"].as<std::string>());
        REQUIRE("foo" == node[value].as<std::string>());
        REQUIRE("foo" == node["value"].as<std::string>());
        REQUIRE(2 == node.size());
    }

    SECTION("NodeTest, SelfReferenceSequence") {
        Node node;
        node[0] = node;
        REQUIRE(node.IsSequence());
        REQUIRE(1 == node.size());
        REQUIRE(node == node[0]);
        REQUIRE(node == node[0][0]);
        REQUIRE(node[0] == node[0][0]);
    }

    SECTION("NodeTest, ValueSelfReferenceMap") {
        Node node;
        node["key"] = node;
        REQUIRE(node.IsMap());
        REQUIRE(1 == node.size());
        REQUIRE(node == node["key"]);
        REQUIRE(node == node["key"]["key"]);
        REQUIRE(node["key"] == node["key"]["key"]);
    }

    SECTION("NodeTest, KeySelfReferenceMap") {
        Node node;
        node[node] = "value";
        REQUIRE(node.IsMap());
        REQUIRE(1 == node.size());
        REQUIRE("value" == node[node].as<std::string>());
    }

    SECTION("NodeTest, SelfReferenceMap") {
        Node node;
        node[node] = node;
        REQUIRE(node.IsMap());
        REQUIRE(1 == node.size());
        REQUIRE(node == node[node]);
        REQUIRE(node == node[node][node]);
        REQUIRE(node[node] == node[node][node]);
    }

    SECTION("NodeTest, TempMapVariable") {
        Node node;
        Node tmp = node["key"];
        tmp = "value";
        REQUIRE(node.IsMap());
        REQUIRE(1 == node.size());
        REQUIRE("value" == node["key"].as<std::string>());
    }

    SECTION("NodeTest, TempMapVariableAlias") {
        Node node;
        Node &tmp = node["key"];
        tmp = node["other"];
        node["other"] = "value";
        REQUIRE(node.IsMap());
        REQUIRE(2 == node.size());
        REQUIRE("value" == node["key"].as<std::string>());
        REQUIRE("value" == node["other"].as<std::string>());
        REQUIRE(node["key"] == node["other"]);
    }

    SECTION("NodeTest, Bool") {
        Node node;
        node[true] = false;
        REQUIRE(node.IsMap());
        REQUIRE(false == node[true].as<bool>());
    }

    SECTION("NodeTest, AutoBoolConversion") {
#ifdef _MSC_VER
#pragma warning(disable : 4800)
#endif
        Node node;
        node["foo"] = "bar";
        ///REQUIRE(static_cast<bool>(node["foo"]));
        ///REQUIRE(!node["monkey"]);
        ///REQUIRE(!!node["foo"]);
    }

    SECTION("NodeTest, FloatingPrecisionFloat") {
        const float x = 0.123456789;
        Node node = Node(x);
        REQUIRE(x == node.as<float>());
    }

    SECTION("NodeTest, FloatingPrecisionPositiveInfinityFloat") {
        if (!std::numeric_limits<float>::has_infinity) {
            return;
        }
        const float x = std::numeric_limits<float>::infinity();
        Node node = Node(x);
        REQUIRE(x == node.as<float>());
    }

    SECTION("NodeTest, FloatingPrecisionNegativeInfinityFloat") {
        if (!std::numeric_limits<float>::has_infinity) {
            return;
        }
        const float x = -std::numeric_limits<float>::infinity();
        Node node = Node(x);
        REQUIRE(x == node.as<float>());
    }

    SECTION("NodeTest, FloatingPrecisionNanFloat") {
        if (!std::numeric_limits<float>::has_quiet_NaN) {
            return;
        }
        const float x = std::numeric_limits<float>::quiet_NaN();
        Node node = Node(x);
        REQUIRE(std::isnan(node.as<float>()));
    }

    SECTION("NodeTest, FloatingPrecisionDouble") {
        const double x = 0.123456789;
        Node node = Node(x);
        REQUIRE(x == node.as<double>());
    }

    SECTION("NodeTest, FloatingPrecisionPositiveInfinityDouble") {
        if (!std::numeric_limits<double>::has_infinity) {
            return;
        }
        const double x = std::numeric_limits<double>::infinity();
        Node node = Node(x);
        REQUIRE(x == node.as<float>());
    }

    SECTION("NodeTest, FloatingPrecisionNegativeInfinityDouble") {
        if (!std::numeric_limits<double>::has_infinity) {
            return;
        }
        const double x = -std::numeric_limits<double>::infinity();
        Node node = Node(x);
        REQUIRE(x == node.as<double>());
    }

    SECTION("NodeTest, FloatingPrecisionNanDouble") {
        if (!std::numeric_limits<double>::has_quiet_NaN) {
            return;
        }
        const double x = std::numeric_limits<double>::quiet_NaN();
        Node node = Node(x);
        REQUIRE(std::isnan(node.as<double>()));
    }

    SECTION("NodeTest, SpaceChar") {
        Node node = Node(' ');
        REQUIRE(' ' == node.as<char>());
    }

    SECTION("NodeTest, CloneNull") {
        Node node;
        ///Node clone = Clone(node);
        ///REQUIRE(NodeType::Null == clone.Type());
    }

    SECTION("NodeTest, KeyNodeExitsScope") {
        Node node;
        {
            Node temp("Hello, world");
            node[temp] = 0;
        }
        for (auto it = node.begin(); it != node.end(); ++it) {
            (void) it;
        }
    }

    SECTION("NodeTest, DefaultNodeStyle") {
        Node node;
        REQUIRE(EmitterStyle::Default == node.style());
    }

    SECTION("NodeTest, AccessNonexistentKeyOnConstNode") {
        Node node;
        node["3"] = "4";
        const Node &other = node;
        //REQUIRE(!other["5"]);
    }
/**
    TEST_F(NodeEmitterTest, SimpleFlowSeqNode) {
        Node node;
        node.SetStyle(EmitterStyle::Flow);
        node.push_back(1.5);
        node.push_back(2.25);
        node.push_back(3.125);

        ExpectOutput("[1.5, 2.25, 3.125]", node);
    }

    TEST_F(NodeEmitterTest, NestFlowSeqNode) {
        Node node, cell0, cell1;

        cell0.push_back(1.5);
        cell0.push_back(2.25);
        cell0.push_back(3.125);

        cell1.push_back(4.5);
        cell1.push_back(5.25);
        cell1.push_back(6.125);

        node.SetStyle(EmitterStyle::Flow);
        node.push_back(cell0);
        node.push_back(cell1);

        ExpectOutput("[[1.5, 2.25, 3.125], [4.5, 5.25, 6.125]]", node);
    }

    TEST_F(NodeEmitterTest, MixBlockFlowSeqNode) {
        Node node, cell0, cell1;

        cell0.SetStyle(EmitterStyle::Flow);
        cell0.push_back(1.5);
        cell0.push_back(2.25);
        cell0.push_back(3.125);

        cell1.push_back(4.5);
        cell1.push_back(5.25);
        cell1.push_back(6.125);

        node.SetStyle(EmitterStyle::Block);
        node.push_back(cell0);
        node.push_back(cell1);

        ExpectOutput("- [1.5, 2.25, 3.125]\n-\n  - 4.5\n  - 5.25\n  - 6.125", node);
    }

    TEST_F(NodeEmitterTest, NestBlockFlowMapListNode) {
        Node node, mapNode, blockNode;

        node.push_back(1.5);
        node.push_back(2.25);
        node.push_back(3.125);

        mapNode.SetStyle(EmitterStyle::Flow);
        mapNode["position"] = node;

        blockNode.push_back(1.0625);
        blockNode.push_back(mapNode);

        ExpectOutput("- 1.0625\n- {position: [1.5, 2.25, 3.125]}", blockNode);
    }

    TEST_F(NodeEmitterTest, NestBlockMixMapListNode) {
        Node node, mapNode, blockNode;

        node.push_back(1.5);
        node.push_back(2.25);
        node.push_back(3.125);

        mapNode.SetStyle(EmitterStyle::Flow);
        mapNode["position"] = node;

        blockNode["scalar"] = 1.0625;
        blockNode["object"] = mapNode;

        ExpectAnyOutput(blockNode,
                        "scalar: 1.0625\nobject: {position: [1.5, 2.25, 3.125]}",
                        "object: {position: [1.5, 2.25, 3.125]}\nscalar: 1.5");
    }

    TEST_F(NodeEmitterTest, NestBlockMapListNode) {
        Node node, mapNode;

        node.push_back(1.5);
        node.push_back(2.25);
        node.push_back(3.125);

        mapNode.SetStyle(EmitterStyle::Block);
        mapNode["position"] = node;

        ExpectOutput("position:\n  - 1.5\n  - 2.25\n  - 3.125", mapNode);
    }

    TEST_F(NodeEmitterTest, NestFlowMapListNode) {
        Node node, mapNode;

        node.push_back(1.5);
        node.push_back(2.25);
        node.push_back(3.125);

        mapNode.SetStyle(EmitterStyle::Flow);
        mapNode["position"] = node;

        ExpectOutput("{position: [1.5, 2.25, 3.125]}", mapNode);
    }*/
}
