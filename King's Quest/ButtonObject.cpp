

#include"ButtonObject.h"

 ButtonObject :: ButtonObject(){
    is_clicked = false;
 }

 ButtonObject :: ~ButtonObject()
 {

 }

void ButtonObject::SetRect(const int& x, const int& y, const int& w, const int& h, const std::string& type) {
        button_rect.x = x;
        button_rect.y = y;
        button_rect.w = w;
        button_rect.h = h;
        button_type = type;
    }

bool ButtonObject :: CheckClick(const SDL_Event& e) {
   if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouse_x = e.button.x;
            int mouse_y = e.button.y;
            if (mouse_x >= button_rect.x && mouse_x <= button_rect.x + button_rect.w &&
                mouse_y >= button_rect.y && mouse_y <= button_rect.y + button_rect.h) {
                if (button_type == "pause") {
                    is_clicked = !is_clicked; // Chuyển đổi trạng thái cho Pause
                } else if (button_type == "continue") {
                    is_clicked = true; // Continue chỉ cần nhấn một lần
                }
                return true;
            }
        }
        return false;
}



void ButtonObject :: Show(SDL_Renderer* des) {
    SDL_Rect renderQuad = { button_rect.x, button_rect.y, button_rect.w, button_rect.h };
    SDL_RenderCopy(des, p_object, NULL, &renderQuad);
}





