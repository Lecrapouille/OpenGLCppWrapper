Dirt texture

  if (keyPressed(GLFW_KEY_F1))
    {
      ERROR("DIRT");
      // Modify the texture: create a chessboard on the first 1/4 part of
      // the first texture.
      GLTexture2D& texture = m_vao_quad.texture2D("texID");
      for (uint32_t u = 0; u < texture.width() / 4; ++u)
        {
          for (uint32_t v = 0; v < texture.height() / 4; ++v)
            {
              unsigned char c = ((((u&0x8)==0)^((v&0x8))==0))*255;
              texture.at(u, v, 0) = c; // R
              texture.at(u, v, 1) = c; // G
              texture.at(u, v, 2) = c; // B
              texture.at(u, v, 3) = 255; // A
            }
        }
      //texture.data().tagAsPending(262144-1);
    }
  else if (keyPressed(GLFW_KEY_F2))
    {
      m_vao_quad.texture2D("texID").load("textures/wooden-crate.jpg");
    }
  else if (keyPressed(GLFW_KEY_F3))
    {
      m_vao_quad.texture2D("texID").load("textures/hazard.png");
    }
   else if (keyPressed(GLFW_KEY_F4))
    {
      m_vao_quad.texture2D("texID").save("/tmp/foo.bmp") ? ERROR("ok") : ERROR("ko");
    }



