#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../include/Logic/MoveTree.hpp"
#include "../include/Logic/Move.hpp"

namespace 
{
    struct MoveTreeFixture 
    {
        MoveTree m_tree;
        MoveTree::Iterator m_iterator;

        MoveTreeFixture(): m_iterator(m_tree.begin()) 
        {
        }

        void insertMoves(const std::vector<std::string>& moves_) 
        {
            for (const auto& move : moves_) 
            {
                m_tree.insertNode(std::make_shared<Move>(move), m_iterator);
            }
        }
    };


    void validateCurrentNode(
        const std::shared_ptr<MoveTreeNode>& currentNode_,
        const std::optional<std::shared_ptr<Move>>& expectedMove_,
        std::optional<int> expectedChildrenCount_,
        const std::optional<std::shared_ptr<Move>>& expectedParentMove_,
        std::optional<int> expectedParentChildrenCount_)
    {
        if (expectedMove_.has_value()) {
            BOOST_CHECK(currentNode_->m_move == expectedMove_.value());
        }
        if (expectedChildrenCount_.has_value()) {
            BOOST_CHECK(currentNode_->m_children.size() == expectedChildrenCount_.value());
        }
        if (expectedParentMove_.has_value()) {
            BOOST_CHECK(currentNode_->m_parent->m_move == expectedParentMove_.value());
        }
        if (expectedParentChildrenCount_.has_value()) {
            BOOST_CHECK(currentNode_->m_parent->m_children.size() == expectedParentChildrenCount_.value());
        }
    }
}

BOOST_FIXTURE_TEST_SUITE(MoveTreeTestSuite, MoveTreeFixture)

BOOST_AUTO_TEST_CASE(testInsertNewNodeAtBeginning) 
{
    // Initially there are no moves
    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 0); 
    
    auto move1 = make_shared<Move>("e2e4");
    m_tree.insertNode(move1, m_iterator);

    validateCurrentNode(m_iterator.getCurrentNode(), move1, 0, std::nullopt, std::nullopt);
    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 1); 
}

BOOST_AUTO_TEST_CASE(testInsertNewNode) 
{
    // Initially there are no moves
    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 0); 
    
    auto move1 = make_shared<Move>("e2e4");
    m_tree.insertNode(move1, m_iterator);
    auto move2 = make_shared<Move>("d7d5");
    m_tree.insertNode(move2, m_iterator);

    validateCurrentNode(m_iterator.getCurrentNode(), move2, 0, move1, 1);
    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 2); 
}

BOOST_AUTO_TEST_CASE(testInsertDuplicatedNode) 
{
    // Initially there are no moves
    BOOST_CHECK(m_tree.getNumberOfMoves() == 0); 
    
    auto initialMove = make_shared<Move>("e2e4");
    m_tree.insertNode(initialMove, m_iterator);
    auto nextMove = make_shared<Move>("d7d5");
    m_tree.insertNode(nextMove, m_iterator);
    m_tree.goToPreviousNode(m_iterator);

    BOOST_CHECK(m_iterator.getCurrentNode()->m_move == initialMove);
    BOOST_CHECK(m_tree.getNumberOfMoves() == 2);
    
    auto duplicatedMove = make_shared<Move>("d7d5");
    m_tree.insertNode(duplicatedMove, m_iterator);

    validateCurrentNode(m_iterator.getCurrentNode(), nextMove, 0, initialMove, 1);
    BOOST_CHECK(m_tree.getNumberOfMoves() == 2);
}

BOOST_AUTO_TEST_CASE(InsertNodeAtGrandParent)
{
    auto initialMove = make_shared<Move>("e2e4");
    auto secondMove = make_shared<Move>("d7d5");
    auto thirdMove = make_shared<Move>("e4d5");
    m_tree.insertNode(initialMove, m_iterator);
    m_tree.insertNode(secondMove, m_iterator);
    m_tree.insertNode(thirdMove, m_iterator);

    BOOST_CHECK(m_iterator.getCurrentNode()->m_move == thirdMove);
    m_iterator.goToParent();
    m_iterator.goToParent();
    auto newVariation = make_shared<Move>("e7e5");
    m_tree.insertNode(newVariation, m_iterator);

    validateCurrentNode(m_iterator.getCurrentNode(), newVariation, 0, initialMove, 2);
    BOOST_CHECK(m_tree.getNumberOfMoves() == 4);
}

BOOST_AUTO_TEST_CASE(GoToNextNode)
{
    auto initialMove = make_shared<Move>("e2e4");
    auto secondMove = make_shared<Move>("d7d5");
    m_tree.insertNode(initialMove, m_iterator);
    m_tree.insertNode(secondMove, m_iterator);

    // Go to an existing variation.
    m_iterator.goToParent();
    m_tree.goToNextNode(0, m_iterator);
    validateCurrentNode(m_iterator.getCurrentNode(), secondMove, std::nullopt, std::nullopt, std::nullopt);

    // Leaf node, we don't do anything.
    m_tree.goToNextNode(0, m_iterator);
    validateCurrentNode(m_iterator.getCurrentNode(), secondMove, std::nullopt, std::nullopt, std::nullopt);
}

BOOST_AUTO_TEST_CASE(testGetNumberOfMoves) 
{
    // Insert some moves and then check the number of moves
    auto move1 = make_shared<Move>("e2e4");
    m_tree.insertNode(move1, m_iterator);
    auto move2 = make_shared<Move>("d7d5");
    m_tree.insertNode(move2, m_iterator);
    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 2); 
}

BOOST_AUTO_TEST_CASE(testGoToPreviousNode)
{
    auto move1 = make_shared<Move>("e2e4");
    auto move2 = make_shared<Move>("d7d5");
    m_tree.insertNode(move1, m_iterator);
    m_tree.insertNode(move2, m_iterator);

    m_tree.goToPreviousNode(m_iterator);
    validateCurrentNode(m_iterator.getCurrentNode(), move1, 1, std::nullopt, std::nullopt);
}

BOOST_AUTO_TEST_CASE(testGetNodeLevel)
{
    auto move1 = make_shared<Move>("e2e4");
    auto move2 = make_shared<Move>("d7d5");
    m_tree.insertNode(move1, m_iterator);
    m_tree.insertNode(move2, m_iterator);

    int level = m_tree.getNodeLevel(m_iterator);
    BOOST_CHECK_EQUAL(level, 2);
}

BOOST_AUTO_TEST_CASE(testClear)
{
    auto move1 = make_shared<Move>("e2e4");
    auto move2 = make_shared<Move>("d7d5");
    m_tree.insertNode(move1, m_iterator);
    m_tree.insertNode(move2, m_iterator);

    m_tree.clear();

    BOOST_CHECK_EQUAL(m_tree.getNumberOfMoves(), 0);
}

BOOST_AUTO_TEST_SUITE_END()
