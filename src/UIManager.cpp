#include "../include/UIManager.hpp"
#include "../include/Utilities/DrawableSf.hpp"
#include "./Ressources/Shader.cpp"
#include "../include/Components/SidePanel.hpp"

class MoveList;

namespace ui {
    UIManager::UIManager(
    Board& board_, 
    MoveTree::Iterator& treeIterator_,
    MoveList& moveList_
    ) :
        m_board(board_),
        m_treeIterator(treeIterator_),
        m_sidePanel(m_window, moveList_, m_showMoveSelectionPanel),
        m_moveSelectionPanel(m_window, m_sidePanel)
    {
        m_window.setFramerateLimit(60);

        // Load ressources
        RessourceManager::loadRessources();

        // Setting window icon
        sf::Image icon;
        icon.loadFromFile(RessourceManager::getIconPath("nw.png"));
        m_window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
        m_window.setPosition({300, 300});

        drawBoardSquares();
        initializeMenuBar();
    };

    void UIManager::draw(
        ClickState& clickState_, 
        DragState& dragState_, 
        ArrowsInfo& arrowsInfo_,
        bool kingChecked_,
        vector<Move>& possibleMoves_,
        bool noMovesAvailable_,
        bool kingIsChecked_)
    {
        drawMenuBar();
        drawBoardSquares();
        drawSidePanel(m_sidePanel);

        if (kingChecked_) drawKingCheckCircle();

        if ((dragState_.pieceIsMoving || clickState_.pieceIsClicked) && clickState_.pSelectedPiece)
        {
            drawCaptureCircles(clickState_.pSelectedPiece, possibleMoves_);
            highlightHoveredSquare(clickState_.pSelectedPiece, clickState_.mousePos, possibleMoves_);
        }
        drawPieces();
        if (dragState_.pieceIsMoving) drawDraggedPiece(clickState_.pSelectedPiece, clickState_.mousePos);
        if (m_transitioningPiece.getIsTransitioning()) {
            drawTransitioningPiece(m_transitioningPiece);
        }
        drawAllArrows(arrowsInfo_.arrows, arrowsInfo_.currArrow);

        if (m_showMoveSelectionPanel)
        {
            drawGrayCover();
            m_moveSelectionPanel.drawMoveSelectionPanel(m_treeIterator);
        }   

        // End conditions
        if (noMovesAvailable_) drawEndResults(kingChecked_);
    }

    void UIManager::drawBoardSquares()
    {
        const sf::Color colours[2] = {{240, 217, 181}, {181, 136, 99}};

        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                // Drawing the colored square
                RectangleShape square = createSquare();
                DrawableSf::drawRectangleSf(square, getWindowXPos(i), getWindowYPos(j), square.getSize(), colours[(i+j)%2]);
                m_window.draw(square);
            }
        }
    }

    void UIManager::initializeMenuBar()
    {
        const vector<string> menuLabels{"Menu", "Reset", "Flip"};

        for (size_t i = 0; i < menuLabels.size(); ++i) m_menuBar.push_back({menuLabels[i], i});
    }

    void UIManager::drawMenuBar()
    {
        constexpr int menuOptionsCount = 3;
        const std::vector<std::string> iconFiles{"dropDownWhite.png", "resetWhite.png", "flipWhite.png"};

        for (size_t i = 0; i < menuOptionsCount; ++i)
        {
            auto texture = RessourceManager::getTexture(iconFiles[i]);
            MenuButton& option = m_menuBar[i];
            option.setSpriteTexture(*texture);

            if (option.getIsColorTransitioning()) option.doColorTransition();

            option.drawMenuButton(m_window);
        }
    }

    void UIManager::drawSidePanel(SidePanel& sidePanel_)
    {
        // Draw the main panels
        RectangleShape mainPanel(Vector2f(g_PANEL_SIZE - 2*g_BORDER_SIZE, g_MAIN_PANEL_HEIGHT - 2*g_BORDER_SIZE));
        RectangleShape southPanel(Vector2f(g_PANEL_SIZE - 2*g_BORDER_SIZE, g_SOUTH_PANEL_HEIGHT));
        mainPanel.setFillColor({50, 50, 50}); // Charcoal
        southPanel.setFillColor({50, 50, 50});
        mainPanel.setPosition(g_WINDOW_SIZE + g_BORDER_SIZE, g_MENUBAR_HEIGHT);
        southPanel.setPosition(g_WINDOW_SIZE + g_BORDER_SIZE, g_MENUBAR_HEIGHT + g_MAIN_PANEL_HEIGHT - g_BORDER_SIZE);

        m_window.draw(mainPanel);
        m_window.draw(southPanel);

        // Draw the content on the panels
        Vector2i position = sf::Mouse::getPosition(m_window);
        coor2d mousePos = {position.x, position.y};
        sidePanel_.drawMoves(mousePos);
    }

    void UIManager::drawGrayCover()
    {
        RectangleShape cover{};
        DrawableSf::drawRectangleSf(
            cover, 0, g_MENUBAR_HEIGHT,
            Vector2f(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE), Color(220, 220, 220, 75)
        );
        m_window.draw(cover);
    }


    void UIManager::highlightHoveredSquare(
        const shared_ptr<Piece>& pSelectedPiece_, 
        const coor2d& mousePos_,
        vector<Move>& possibleMoves_)
    {
        const Color colours[2] = {{173, 176, 134}, {100, 111, 64}};

        for (auto& move: possibleMoves_)
        {
            auto [j, i] = move.getTarget();
            if (m_board.isFlipped()) {i = 7-i; j = 7-j;}
            if (move.getSelectedPiece() != pSelectedPiece_) continue;
            int xPos = getTileXPos(mousePos_), yPos = getTileYPos(mousePos_);

            if (i == xPos && j == yPos)
            {
                // Currently hovering a square where the piece can move
                RectangleShape square = createSquare();
                DrawableSf::drawRectangleSf(square, getWindowXPos(i), getWindowYPos(j), square.getSize(), colours[(i+j)%2]);
                m_window.draw(square);
            }
        }
    }

    void UIManager::drawCaptureCircles(
        const shared_ptr<Piece>& pSelectedPiece_,
        vector<Move>& possibleMoves_)
    {
        for (auto& move: possibleMoves_)
        {
            auto [j, i] = move.getTarget();

            if (move.getSelectedPiece() != pSelectedPiece_) continue;
            bool isEmpty = m_board.getBoardTile(i, j).get() == nullptr;
            const shared_ptr<Texture> t = RessourceManager::getTexture(isEmpty? "circle.png": "empty_circle.png");

            if (!t) return;
            Sprite circle(*t);

            if (isEmpty) circle.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            else circle.setScale(g_SPECIAL_SCALE, g_SPECIAL_SCALE);

            if (m_board.isFlipped()) {i = 7-i; j = 7-j;}
            circle.setPosition(getWindowXPos(i), getWindowYPos(j));
            m_window.draw(circle);
        }
    }

    void UIManager::drawPieces()
    {
        for (size_t i = 0; i < 8; ++i)
        {
            for (size_t j = 0; j < 8; ++j)
            {
                shared_ptr<Piece> piece = m_board.getBoardTile(i, j);
                if (!piece) continue;

                // Do not draw transitioning pieces
                if (m_transitioningPiece.getIsTransitioning())
                {
                    if (
                        piece == m_transitioningPiece.getPiece() ||
                        piece == m_transitioningPiece.getCapturedPiece() ||
                        piece == m_transitioningPiece.getSecondPiece() ||
                        piece == m_transitioningPiece.getPromotingPiece()
                    ) continue;
                }

                shared_ptr<Texture> t = RessourceManager::getTexture(piece);
                if (!t) return;

                Sprite s(*t);
                s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
                s.setPosition(getWindowXPos(m_board.isFlipped() ? (7-i): i), getWindowYPos(m_board.isFlipped()? (7-j): j));
                m_window.draw(s);
            }
        }
    }

    void UIManager::drawDraggedPiece(const shared_ptr<Piece>& pSelectedPiece_, const coor2d& mousePos_)
    {
        if (!pSelectedPiece_) return; // Safety check
        shared_ptr<Texture> t = RessourceManager::getTexture(pSelectedPiece_);
        shared_ptr<Texture> tBefore = RessourceManager::getTexture(pSelectedPiece_);

        if (!t || !tBefore) return;
        Sprite s(*t), sBefore(*tBefore);
        s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        sBefore.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        s.setPosition(mousePos_.first, mousePos_.second);
        sBefore.setPosition(
            getWindowXPos(m_board.isFlipped()? 7-pSelectedPiece_->getY(): pSelectedPiece_->getY()),
            getWindowYPos(m_board.isFlipped()? 7-pSelectedPiece_->getX(): pSelectedPiece_->getX())
        );

        s.setOrigin(g_SPRITE_SIZE/2, g_SPRITE_SIZE/2);
        sBefore.setColor({255, 255, 255, 100});
        m_window.draw(sBefore);
        m_window.draw(s);
    }

    void UIManager::drawAllArrows(vector<Arrow>& arrows_, const Arrow& currArrow_)
    {
        if (arrows_.empty()) return;
        arrows_.push_back(currArrow_);

        for (auto& arrow: arrows_)
        {
            if (!arrow.isDrawable()) continue;

            shared_ptr<Texture> t = RessourceManager::getTexture(arrow.getFilename());
            if (!t) return;
            Sprite s(*t);
            const coor2d arrowOrigin = arrow.getFormattedOrigin();

            if (arrow.isLArrow())
            {
                s.setOrigin(g_CELL_SIZE / 2, s.getLocalBounds().height - g_CELL_SIZE / 2);
                s.setPosition(arrowOrigin.first, arrowOrigin.second);
            }
            else
            {
                s.setOrigin(0, s.getLocalBounds().height / 2);
                s.setPosition(arrowOrigin.first, arrowOrigin.second);
            }
            s.rotate(arrow.getRotation());
            m_window.draw(s);
        }
        arrows_.pop_back();
    }

    void UIManager::drawKingCheckCircle()
    {
        Shader shader;
        shader.loadFromMemory(VertexShader, RadialGradient);
        shader.setUniform("windowHeight", (float) m_window.getSize().y);

        shared_ptr<King> king = m_board.getKing();
        CircleShape c(g_CELL_SIZE / 2);
        c.setFillColor(Color::Transparent);

        int x = m_board.isFlipped()? 7-king->getY(): king->getY();
        int y = m_board.isFlipped()? 7-king->getX(): king->getX();
        c.setPosition(getWindowXPos(x), getWindowYPos(y));
        shader.setUniform("color", Glsl::Vec4(1.f, 0.f, 0.f, 1.f));
        shader.setUniform("center", Vector2f(
            c.getPosition().x + c.getRadius(), c.getPosition().y + + c.getRadius()
        ));
        shader.setUniform("radius", c.getRadius());
        shader.setUniform("expand", 0.15f);

        m_window.draw(c, &shader);
    }

    void UIManager::drawEndResults(bool kingChecked_)
    {
        // Checkmate
        if (kingChecked_)
        {
            shared_ptr<King> losing = m_board.getKing();
            Texture t; t.loadFromFile(RessourceManager::getIconPath("checkmate.png"));
            Sprite checkmate(t);
            checkmate.setColor({255, 255, 255, 200});
            checkmate.setScale(g_SPECIAL_SCALE / 2, g_SPECIAL_SCALE / 2);
            checkmate.setOrigin(40, 40);
            checkmate.setPosition(
                getWindowXPos(m_board.isFlipped()? 7-losing->getY(): losing->getY()) + g_CELL_SIZE,
                getWindowYPos(m_board.isFlipped()? 7-losing->getX(): losing->getX())
            );
            m_window.draw(checkmate);
            return;
        }
    }

    void UIManager::drawTransitioningPiece(PieceTransition& piece_)
    {
        shared_ptr<Piece> captured = piece_.getCapturedPiece();
        piece_.move(m_board);
        shared_ptr<Texture> t = RessourceManager::getTexture(piece_.getPiece());
        if (!t) return;

        // Draw captured piece while transition is happening
        if (captured)
        {
            shared_ptr<Texture> t2 = RessourceManager::getTexture(captured);
            if (!t2) return;

            Sprite s2(*t2);
            s2.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            s2.setPosition(piece_.getCapturedX(), piece_.getCapturedY());

            uint8_t percentage = static_cast<uint8_t>(piece_.getPercentageLeft() * 255);
            if (piece_.isUndo()) percentage = static_cast<uint8_t>(255-percentage);
            s2.setColor({255, 255, 255, percentage});
            m_window.draw(s2);
        }

        // If we castle, draw the rook first so it appears under the king
        if (piece_.getSecondPiece()) {
            shared_ptr<Texture> t2 = RessourceManager::getTexture(piece_.getSecondPiece());
            Sprite s2(*t2);
            s2.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            s2.setPosition(piece_.getSecondCurrPos().first, piece_.getSecondCurrPos().second);
            m_window.draw(s2);
        }

        Sprite s(*t);
        s.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        s.setPosition(piece_.getCurrPos().first, piece_.getCurrPos().second);
        m_window.draw(s);
    }

    void UIManager::handleSidePanelMoveBox(const coor2d& mousePos_)
    {
        if (!m_showMoveSelectionPanel) m_sidePanel.handleMoveBoxClicked(mousePos_);
    }
    bool UIManager::ignoreInputWhenSelectionPanelIsActive(const coor2d& mousePos_) const
    {
        return m_showMoveSelectionPanel && !m_moveSelectionPanel.isHowered(mousePos_);
    }

    int getTileXPos(const coor2d& pos_, bool isFlipped_)
    { 
        int cellPos = pos_.first / g_CELL_SIZE; 
        return isFlipped_ ? 7 - cellPos : cellPos;
    }
    int getTileYPos(const coor2d& pos_, bool isFlipped_)
    {
        int cellPos = (pos_.second - g_MENUBAR_HEIGHT) / g_CELL_SIZE;
        
        if (pos_.second < g_MENUBAR_HEIGHT) return -1;
        return isFlipped_ ? 7 - cellPos : cellPos;
    }
};