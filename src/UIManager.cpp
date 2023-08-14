#include "../include/UIManager.hpp"
#include "../include/Utilities/SFDrawUtil.hpp"
#include "./Ressources/Shader.cpp"
#include "../include/Components/SidePanel.hpp"

class MoveTreeManager;

namespace ui {
    UIManager::UIManager(
    Board& board_, 
    MoveTreeManager& moveList_
    ) :
        m_board(board_),
        m_moveTreeManager(moveList_),
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
        ArrowsInfo& arrowsInfo_)
    {
        // Note that order of function calls in this function is important
        // otherwise drawing is affected negatively.
        drawMenuBar();
        drawBoardSquares();
        drawSidePanel();

        if (m_board.isKingChecked()) drawKingCheckCircle();

        const bool needToDrawCirclesAndHighlightSquares = (dragState_.pieceIsMoving || clickState_.pieceIsClicked) && clickState_.pSelectedPiece;
        if (needToDrawCirclesAndHighlightSquares)
        {
            drawCaptureCircles(clickState_.pSelectedPiece, m_board.getAllCurrentlyAvailableMoves());
            highlightHoveredSquare(clickState_.pSelectedPiece, clickState_.mousePos,  m_board.getAllCurrentlyAvailableMoves());
        }
        highlightLastMove();
        drawPieces();
        if (dragState_.pieceIsMoving) drawDraggedPiece(clickState_.pSelectedPiece, clickState_.mousePos);
        if (m_moveTreeManager.getTransitioningPiece().getIsTransitioning()) {
            drawTransitioningPiece(m_moveTreeManager.getTransitioningPiece());
        }
        drawAllArrows(arrowsInfo_.arrows, arrowsInfo_.currArrow);

        if (m_showMoveSelectionPanel)
        {
            drawGrayCover();
            m_moveSelectionPanel.drawMoveSelectionPanel(m_moveTreeManager.getIterator());
        }   

        // End conditions
        if (m_board.areThereNoMovesAvailableAtCurrentPosition()) 
        {
            drawEndResults(m_board.isKingChecked());
        }
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
                SFDrawUtil::drawRectangleSf(square, getWindowXPos(i), getWindowYPos(j), square.getSize(), colours[(i+j)%2]);
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

    void UIManager::drawSidePanel()
    {
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
        m_sidePanel.drawMoves(
            m_moveTreeManager.getMoveTreeDisplayHandler().generateMoveInfo(),
            mousePos);
    }

    void UIManager::drawGrayCover()
    {
        RectangleShape cover{};
        SFDrawUtil::drawRectangleSf(
            cover, 0, g_MENUBAR_HEIGHT,
            Vector2f(g_WINDOW_SIZE + g_PANEL_SIZE, g_WINDOW_SIZE), Color(220, 220, 220, 75)
        );
        m_window.draw(cover);
    }


    void UIManager::highlightHoveredSquare(
        const shared_ptr<Piece>& pSelectedPiece_, 
        const coor2d& mousePos_,
        const vector<Move>& possibleMoves_)
    {
        const Color colours[2] = {{173, 176, 134}, {100, 111, 64}};

        for (auto& move: possibleMoves_)
        {
            auto [filePiece, rankPiece] = move.getTarget();
            if (m_board.isFlipped()) 
            {
                rankPiece = 7 - rankPiece; 
                filePiece = 7 - filePiece;
            }
            if (move.getSelectedPiece() != pSelectedPiece_) continue;
            int fileMouse = getFile(mousePos_);
            int rankMouse = getRank(mousePos_);

            if (filePiece == fileMouse && rankPiece == rankMouse)
            {
                // Currently hovering a square where the piece can move
                RectangleShape square = createSquare();
                SFDrawUtil::drawRectangleSf(
                    square, 
                    getWindowXPos(filePiece), 
                    getWindowYPos(rankPiece), 
                    square.getSize(), 
                    colours[(rankPiece + filePiece) % 2]);
                m_window.draw(square);
            }
        }
    }

    void UIManager::drawCaptureCircles(
        const shared_ptr<Piece>& pSelectedPiece_,
        const vector<Move>& possibleMoves_)
    {
        for (auto& move: possibleMoves_)
        {
            auto [file, rank] = move.getTarget();

            if (move.getSelectedPiece() != pSelectedPiece_) continue;
            bool isEmpty = m_board.getBoardTile(file, rank).get() == nullptr;
            auto texture = RessourceManager::getTexture(isEmpty? "circle.png": "empty_circle.png");

            if (!texture) return;
            Sprite circle(*texture);

            if (isEmpty) circle.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            else circle.setScale(g_SPECIAL_SCALE, g_SPECIAL_SCALE);

            if (m_board.isFlipped()) {file = 7-file; rank = 7-rank;}
            circle.setPosition(getWindowXPos(file), getWindowYPos(rank));
            m_window.draw(circle);
        }
    }

    void UIManager::highlightLastMove()
    {
        shared_ptr<Move> move = m_moveTreeManager.getIterator()->m_move;
        if (!move) return;
        
        RectangleShape squareBefore = ui::createSquare();
        RectangleShape squareAfter = ui::createSquare();

        Color colorInit = ((move->getInit().first + move->getInit().second) % 2)
                        ? Color(170, 162, 58)
                        : Color(205, 210, 106);
        Color colorTarget = ((move->getTarget().first + move->getTarget().second) % 2)
                        ? Color(170, 162, 58)
                        : Color(205, 210, 106);
        squareBefore.setFillColor(colorInit);
        squareAfter.setFillColor(colorTarget);

        squareBefore.setPosition(
            ui::getWindowXPos(m_board.isFlipped() ? 7-move->getInit().first: move->getInit().first),
            ui::getWindowYPos(m_board.isFlipped() ? 7-move->getInit().second: move->getInit().second)
        );
        squareAfter.setPosition(
            ui::getWindowXPos(m_board.isFlipped() ? 7-move->getTarget().first: move->getTarget().first),
            ui::getWindowYPos(m_board.isFlipped() ? 7-move->getTarget().second: move->getTarget().second)
        );

        m_window.draw(squareBefore);
        m_window.draw(squareAfter);
    }

    void UIManager::drawPieces()
    {
        for (size_t row = 0; row < 8; ++row)
        {
            for (size_t file = 0; file < 8; ++file)
            {
                shared_ptr<Piece>& piece = m_board.getBoardTile(file, row);
                if (!piece) continue;

                // Do not draw transitioning pieces
                auto& pieceTransition = m_moveTreeManager.getTransitioningPiece();
                if (pieceTransition.getIsTransitioning())
                {
                    if (
                        piece == pieceTransition.getPiece() ||
                        piece == pieceTransition.getCapturedPiece() ||
                        piece == pieceTransition.getSecondPiece() ||
                        piece == pieceTransition.getPromotingPiece()
                    ) continue;
                }

                auto texture = RessourceManager::getTexture(piece);
                if (!texture) return;

                Sprite sprite(*texture);
                sprite.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
                sprite.setPosition(
                    getWindowXPos(m_board.isFlipped() ? (7 - file): file), 
                    getWindowYPos(m_board.isFlipped() ? (7 - row): row));
                m_window.draw(sprite);
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
            getWindowXPos(m_board.isFlipped()? 7-pSelectedPiece_->getFile(): pSelectedPiece_->getFile()),
            getWindowYPos(m_board.isFlipped()? 7-pSelectedPiece_->getRank(): pSelectedPiece_->getRank())
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

            auto texture = RessourceManager::getTexture(arrow.getFilename());
            if (!texture) return;
            Sprite sprite(*texture);
            const coor2d& arrowOrigin = arrow.getFormattedOrigin();

            if (arrow.isLArrow())
            {
                sprite.setOrigin(g_CELL_SIZE / 2, sprite.getLocalBounds().height - g_CELL_SIZE / 2);
                sprite.setPosition(arrowOrigin.first, arrowOrigin.second);
            }
            else
            {
                sprite.setOrigin(0, sprite.getLocalBounds().height / 2);
                sprite.setPosition(arrowOrigin.first, arrowOrigin.second);
            }
            sprite.rotate(arrow.getRotation());
            m_window.draw(sprite);
        }
        arrows_.pop_back();
    }

    void UIManager::drawKingCheckCircle()
    {
        Shader shader;
        shader.loadFromMemory(VertexShader, RadialGradient);
        shader.setUniform("windowHeight", (float) m_window.getSize().y);

        const auto& king = m_board.getKing();
        CircleShape circle(g_CELL_SIZE / 2);
        circle.setFillColor(Color::Transparent);

        int rank = m_board.isFlipped()? 7-king->getRank(): king->getRank();
        int file = m_board.isFlipped()? 7-king->getFile(): king->getFile();
        circle.setPosition(getWindowXPos(file), getWindowYPos(rank));
        shader.setUniform("color", Glsl::Vec4(1.f, 0.f, 0.f, 1.f));
        shader.setUniform("center", Vector2f(
            circle.getPosition().x + circle.getRadius(), circle.getPosition().y + circle.getRadius()
        ));
        shader.setUniform("radius", circle.getRadius());
        shader.setUniform("expand", 0.15f);

        m_window.draw(circle, &shader);
    }

    void UIManager::drawEndResults(bool isKingChecked_)
    {
        // Checkmate
        if (isKingChecked_)
        {
            const auto& king = m_board.getKing();
            Texture texture; 
            texture.loadFromFile(RessourceManager::getIconPath("checkmate.png"));

            Sprite checkmate(texture);
            checkmate.setColor({255, 255, 255, 200});
            checkmate.setScale(g_SPECIAL_SCALE / 2, g_SPECIAL_SCALE / 2);
            checkmate.setOrigin(40, 40);
            checkmate.setPosition(
                getWindowXPos(m_board.isFlipped()? 7-king->getFile(): king->getFile()) + g_CELL_SIZE,
                getWindowYPos(m_board.isFlipped()? 7-king->getRank(): king->getRank())
            );
            m_window.draw(checkmate);
            return;
        }
    }

    void UIManager::drawTransitioningPiece(PieceTransition& piece_)
    {
        const shared_ptr<Piece>& captured = piece_.getCapturedPiece();
        piece_.move();
        
        auto texture1 = RessourceManager::getTexture(piece_.getPiece());
        if (!texture1) return;

        // Draw captured piece while transition is happening
        if (captured)
        {
            auto texture2 = RessourceManager::getTexture(captured);
            if (!texture2) return;

            Sprite sprite2(*texture2);
            sprite2.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            sprite2.setPosition(piece_.getCapturedX(), piece_.getCapturedY());

            uint8_t percentage = static_cast<uint8_t>(piece_.getPercentageLeft() * 255);
            if (piece_.isUndo()) percentage = static_cast<uint8_t>(255-percentage);
            sprite2.setColor({255, 255, 255, percentage});
            m_window.draw(sprite2);
        }

        // If we castle, draw the rook first so it appears under the king
        if (piece_.getSecondPiece()) {
            shared_ptr<Texture> t2 = RessourceManager::getTexture(piece_.getSecondPiece());
            Sprite sprite2(*t2);
            sprite2.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
            sprite2.setPosition(piece_.getSecondCurrPos().first, piece_.getSecondCurrPos().second);
            m_window.draw(sprite2);
        }

        Sprite sprite1(*texture1);
        sprite1.setScale(g_SPRITE_SCALE, g_SPRITE_SCALE);
        sprite1.setPosition(piece_.getCurrPos().first, piece_.getCurrPos().second);
        m_window.draw(sprite1);
    }

    void UIManager::handleSidePanelMoveBoxClick(const coor2d& mousePos_)
    {
        if (!m_showMoveSelectionPanel) m_sidePanel.handleMoveBoxClicked(mousePos_);
    }

    bool UIManager::ignoreInputWhenSelectionPanelIsActive(const coor2d& mousePos_) const
    {
        return m_showMoveSelectionPanel && !m_moveSelectionPanel.isHowered(mousePos_);
    }

    void UIManager::resetUserInputStatesAfterNewMove(ClickState& clickState_, DragState& dragState_)
    {
        clickState_.pSelectedPiece.reset();
        clickState_.pieceIsClicked = false;
        clickState_.mousePos = {0, 0};
        dragState_.pieceIsMoving = false;
    }

    int getFile(const coor2d& pos_, bool isFlipped_)
    { 
        int cellPos = pos_.first / g_CELL_SIZE; 
        return isFlipped_ ? 7 - cellPos : cellPos;
    }
    int getRank(const coor2d& pos_, bool isFlipped_)
    {
        int cellPos = (pos_.second - g_MENUBAR_HEIGHT) / g_CELL_SIZE;
        
        if (pos_.second < g_MENUBAR_HEIGHT) return -1;
        return isFlipped_ ? 7 - cellPos : cellPos;
    }

};