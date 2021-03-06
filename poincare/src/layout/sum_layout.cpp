#include "sum_layout.h"
#include <string.h>
#include <assert.h>

namespace Poincare {

const uint8_t symbolPixel[SumLayout::k_symbolHeight][SumLayout::k_symbolWidth] = {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
  {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0xFF, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF},
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
};

void SumLayout::render(KDContext * ctx, KDPoint p, KDColor expressionColor, KDColor backgroundColor) {
  KDSize upperBoundSize = m_upperBoundLayout->size();
  KDSize lowerBoundSize = m_lowerBoundLayout->size();
  KDColor workingBuffer[k_symbolWidth*k_symbolHeight];
  KDRect symbolFrame(p.x() + max(max(0, (upperBoundSize.width()-k_symbolWidth)/2), (lowerBoundSize.width()-k_symbolWidth)/2),
    p.y() + max(upperBoundSize.height()+k_boundHeightMargin, m_argumentLayout->baseline()-k_symbolHeight),
    k_symbolWidth, k_symbolHeight);
  ctx->blendRectWithMask(symbolFrame, expressionColor, (const uint8_t *)symbolPixel, (KDColor *)workingBuffer);
}

}
