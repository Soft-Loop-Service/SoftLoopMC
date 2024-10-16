
#include "./intermediate.hpp"

namespace LanguageIntermediateSystem
{
    void Intermediate ::intermediateComponentEffect(string component_name)
    {
        printf("INTER | %-20s c-name:%-20s\n", "[C] effect", component_name.c_str());
    }
    void Intermediate ::intermediateComponentLink(string component_name)
    {
        printf("INTER | %-20s c-name:%-20s\n", "[C] link", component_name.c_str());
    }
    void Intermediate ::recursionComponent(int component_index)
    {
        printf("INTER | %-20s node:%d\n", "R-C", component_index);
        LanguageSpecifications::Scl::vSclLiteral scl_literal_effect = {};
        scl_expr.searchSclLiteral(scl_literal_effect, component_index, "Effect");

        LanguageSpecifications::Scl::vSclLiteral scl_literal_link = {};
        scl_expr.searchSclLiteral(scl_literal_link, component_index, "Link");

        for (int i = 0; i < scl_literal_effect.size(); i++)
        {
            string literal = scl_literal_effect[i].getLiteral();
            intermediateComponentEffect(literal);
        }

        for (int i = 0; i < scl_literal_link.size(); i++)
        {
            string literal = scl_literal_link[i].getLiteral();
            intermediateComponentLink(literal);
            recursionComponentArea(literal);
        }
    }

    void Intermediate ::recursionComponentArea(string component_name)
    {
        vint area_components = scl_expr.searchContainers("component", "Area", component_name);
        printf("INTER | %-20s node:%d\n", "C-AREA", area_components.size());

        TransSystem::HtmlKit html_kit;
        html_kit.setTag("div");
        html_kit.pushAttribute("id", component_name);
        trans_kit.pushHtmlTree(html_kit);

        for (int i = 0; i < area_components.size(); i++)
        {
            int component_index = area_components[i];
            printf("INTER | %-20s node:%d\n", "C-AREA-L", component_index);
            recursionComponent(component_index);
        }
    }

    Intermediate ::Intermediate(LanguageSpecifications::Scl::SclExpr arg_scl_expr, TransSystem::TransKit arg_trans_kit)
    {
        scl_expr = arg_scl_expr;
        trans_kit = arg_trans_kit;
    }

    TransSystem::TransKit Intermediate ::getTransKit()
    {
        return trans_kit;
    }

    void intermediate(LanguageSpecifications::Scl::SclExpr scl_expr, Intermediate &echo_system)
    {

        vint page_components = scl_expr.searchContainers("component", "Page");
        printf("INTER | %-20s size:%d\n", "----", page_components.size());

        for (int i = 0; i < page_components.size(); i++)
        {
            int component_index = page_components[i];
            printf("INTER | %-20s node:%d\n", "PAGE", component_index);
            echo_system.recursionComponent(component_index);
        }

        TransSystem::TransKit tk = echo_system.getTransKit();
        TransSystem::vHtmlKit html_kit_tree = tk.getHtmlTree();
        TransSystem::vCssKit css_kit_tree = tk.getCssTree();
        TransSystem::vScriptKit script_kit_tree = tk.getScriptTree();

        printf("TK-HTML | %-20s size:%d\n", "----", html_kit_tree.size());
        for (int i = 0; i < html_kit_tree.size(); i++)
        {
            printf("TK-HTML | ");
            TransSystem::vHtmlKit v_dummy = {};
            string t = html_kit_tree[i].generate(v_dummy);
            printf("%s\n", t.c_str());
        }
        printf("TK-CSS | %-20s size:%d\n", "----", css_kit_tree.size());
        for (int i = 0; i < css_kit_tree.size(); i++)
        {
            printf("TK-CSS | ");
            TransSystem::vCssKit v_dummy = {};
            string t = css_kit_tree[i].generate(v_dummy);
            printf("%s\n", t.c_str());
        }
        printf("TK-SCRIPT | %-20s size:%d\n", "----", script_kit_tree.size());
        for (int i = 0; i < script_kit_tree.size(); i++)
        {
            printf("TK-SCRIPT | ");
            TransSystem::vScriptKit v_dummy = {};
            string t = script_kit_tree[i].generate(v_dummy);
            printf("%s\n", t.c_str());
        }

        html_kit_tree[0].generate(html_kit_tree);
    }

};
