#include "statement.hpp"
#include <fstream>
#include <memory>

void subject2(const char* filename)
{
  std::ifstream input_file(filename, std::ios_base::binary);

  if (input_file.good() == 0)
  {
    throw std::invalid_argument("ERROR! Unrecognized file name!\n");
  }

  size_t size = 0;
  size_t content_capacity = 64;
  std::unique_ptr<char[], decltype(&free)> contents(static_cast<char*>(malloc(content_capacity)), free);

  while (input_file)
  {
    input_file.read(&contents[size], content_capacity - size);
    size += input_file.gcount();

    if (size == content_capacity)
    {
      content_capacity *= 2;
      auto temp_content = contents.release();
      contents.reset(static_cast<char*>(std::realloc(temp_content, content_capacity)));

      if (!contents)
      {
        throw std::runtime_error("ERROR! Memory allocation is abnormal!\n");
      }
    }
  }

  input_file.close();

  if (input_file.is_open())
  {
    throw std::ios_base::failure("ERROR! Can not close file!\n");
  }

  std::vector<char> vec(&contents[0], &contents[size]);

  for (char& it : vec) // width
  {
    std::cout << it;
  }
}
