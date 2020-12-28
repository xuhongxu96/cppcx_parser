using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class FunctionDeclarationAnalyzer
    {
        public class Parameter
        {
            public string Type { get; set; }
            public string Name { get; set; }
            public string DefaultValue { get; set; }
        }

        public class FunctionDeclaration
        {
            public string Name { get; set; }
            public string ReturnType { get; set; }
            public List<Parameter> Parameters { get; init; } = new List<Parameter>();
        }

        private Parameter VisitParameterDeclaration([NotNull] CPPCXParser.ParameterDeclarationContext context)
        {
            return new Parameter
            {
                Type = context.declSpecifierSeq().declSpecifier()[0].GetText(),
                Name = context.declSpecifierSeq().declSpecifier()[1].GetText().Trim('*', '^', '&'),
            };
        }

        public FunctionDeclaration Visit([NotNull] CPPCXParser.SimpleDeclarationContext context)
        {
            var retType = context.declSpecifierSeq().GetText();

            var nameAndParms = context.initDeclaratorList().initDeclarator()[0].declarator().pointerDeclarator().noPointerDeclarator();

            var name = nameAndParms.noPointerDeclarator().GetText();
            var parameterDecls = nameAndParms.parametersAndQualifiers()?.parameterDeclarationClause()?.parameterDeclarationList()?.parameterDeclaration();

            var res = new FunctionDeclaration { Name = name, ReturnType = retType };

            if (parameterDecls == null)
            {
                return res;
            }

            foreach (var parameterDecl in parameterDecls)
            {
                res.Parameters.Add(VisitParameterDeclaration(parameterDecl));
            }

            return res;
        }
    }
}
