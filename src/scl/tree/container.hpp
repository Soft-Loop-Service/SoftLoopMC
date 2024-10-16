
#ifndef __SCLTCR
#define __SCLTCR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "./../../definition.hpp"
#include "./../../lexical/lexical_analysis.hpp"
#include "./../../syntactic/syntactic_analysis.hpp"

// SCL解析系
namespace LanguageSpecifications
{
    using namespace Syntactic;

    namespace Scl
    {
        typedef SyntacticTreeNode TN;

        class SclContainer;
        typedef std::vector<SclContainer> vSclContainer;

        class SclContainer
        {
        private:
            int tree_node_index;
            int container_index;
            string container_name;
            string container_major_type;
            string container_minor_type;

            vint variables_index;
            // vint contact_container_index_list; // vSclContainerのcontact index
            // vstring contact_list;

        public:
            /// @brief SclContainerコントラスタ
            SclContainer();
            /// @brief SclContainerコントラスタ
            /// @param 抽象構文木でのcomponentノード番号
            void setTreeNodeIndex(int);
            void setContainerIndex(int);
            void setContainerName(string);
            void setContainerType(string, string);
            void setContainerMajorType(string);
            void setContainerMinorType(string);

            int getTreeNodeIndex();
            int getContainerIndex();
            string getContainerName();
            string getContainerMajorType();
            string getContainerMinorType();

            void setVariable(int);
            void setVariable(vint);
            void addVariable(int);
            void addVariable(vint);
            int getVariable(int);
            vint getVariables();

            void debug()
            {
                printf("[COMPONENT] | c-name:%40s | cma-type:%20s | cmi-type:%20s | c-index:%2d | tree-index:%2d | \n",
                       container_name.c_str(), container_major_type.c_str(), container_minor_type.c_str(), container_index, tree_node_index);

                printf("          - | {");

                for (int i = 0; i < variables_index.size(); i++)
                {
                    printf(" %d ", variables_index[i]);
                }

                printf("}\n");

                // printf(" [CONTAXT] | ");
                // for (int i = 0; i < contact_list.size(); i++)
                // {
                //     printf(" %20s ", contact_list[i].c_str());
                // }
                // printf("\n");
            }
        };

    }

};

#endif