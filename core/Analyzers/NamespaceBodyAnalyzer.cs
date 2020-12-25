using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Tree;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace cppcx.Core.Analyzers
{
    public class NamespaceBodyAnalyzer : CPPCXParserBaseVisitor<NamespaceBodyAnalyzer.NamespaceBody>
    {
        public class GlobalVariableDeclaration
        {
            public CPPCXParser.SimpleDeclarationContext Context;
            public string Text => Context?.GetText();
            public override string ToString() => Text;
        }

        public class ClassDeclaration
        {
            public CPPCXParser.ClassSpecifierContext Context;
            public string Text => Context?.GetText();
            public override string ToString() => Text;
        }

        public class FunctionDeclaration
        {
            public CPPCXParser.SimpleDeclarationContext Context;
            public string Text => Context?.GetText();
            public override string ToString() => Text;
        }

        public class FunctionDefinition
        {
            public CPPCXParser.FunctionDefinitionContext Context;
            public string Text => Context?.GetText();
            public override string ToString() => Text;
        }

        public class NamespaceBody
        {
            public List<GlobalVariableDeclaration> GlobalVariables { get; init; } = new List<GlobalVariableDeclaration>();
            public List<ClassDeclaration> Classes { get; init; } = new List<ClassDeclaration>();
            public List<FunctionDeclaration> FunctionDeclarations { get; init; } = new List<FunctionDeclaration>();
            public List<FunctionDefinition> FunctionDefinitions { get; init; } = new List<FunctionDefinition>();

            public override string ToString()
            {
                return JsonSerializer.Serialize(this, new JsonSerializerOptions { WriteIndented = true });
            }

            public void MergeFrom(NamespaceBody other)
            {
                GlobalVariables.AddRange(other.GlobalVariables);
                Classes.AddRange(other.Classes);
                FunctionDeclarations.AddRange(other.FunctionDeclarations);
                FunctionDefinitions.AddRange(other.FunctionDefinitions);
            }
        }

        private enum DeclarationType
        {
            Unknown, Variable, Function
        }

        private DeclarationType IsVariableDeclaration(CPPCXParser.SimpleDeclarationContext context)
        {
            var decls = context.declSpecifierSeq()?.declSpecifier();
            if (decls == null) return DeclarationType.Unknown;

            var initDecl = context.initDeclaratorList();
            if (initDecl == null && decls.Length < 2) return DeclarationType.Unknown;


            CPPCXParser.DeclSpecifierContext typeDecl;

            if (initDecl != null)
            {
                Debug.Assert(decls.Length == 1, "If it's definition, storage class shouldn't appear");
                typeDecl = decls[0];
            }
            else
            {
                typeDecl = decls[^2];
            }

            if (typeDecl.typeSpecifier()?.trailingTypeSpecifier()?.simpleTypeSpecifier() == null)
            {
                return DeclarationType.Unknown;
            }

            if (initDecl?.initDeclarator() != null
                && initDecl.initDeclarator().Length == 1
                && initDecl.initDeclarator()[0]?.declarator()?.pointerDeclarator()?.noPointerDeclarator()?.parametersAndQualifiers() != null)
            {
                return DeclarationType.Function;
            }

            return DeclarationType.Variable;
        }

        public override NamespaceBody VisitClassSpecifier([NotNull] CPPCXParser.ClassSpecifierContext context)
        {
            return new NamespaceBody
            {
                Classes = new List<ClassDeclaration>
                {
                    new ClassDeclaration { Context = context },
                }
            };
        }

        public override NamespaceBody VisitSimpleDeclaration([NotNull] CPPCXParser.SimpleDeclarationContext context)
        {
            var result = new NamespaceBody();
            switch (IsVariableDeclaration(context))
            {
                case DeclarationType.Variable:
                    return new NamespaceBody
                    {
                        GlobalVariables = new List<GlobalVariableDeclaration>
                        {
                            new GlobalVariableDeclaration { Context = context }
                        }
                    };
                case DeclarationType.Function:
                    return new NamespaceBody
                    {
                        FunctionDeclarations = new List<FunctionDeclaration>
                        {
                            new FunctionDeclaration { Context = context }
                        }
                    };
                default:
                    return VisitChildren(context);
            }
        }

        public override NamespaceBody VisitFunctionDefinition([NotNull] CPPCXParser.FunctionDefinitionContext context)
        {
            return new NamespaceBody
            {
                FunctionDefinitions = new List<FunctionDefinition>
                {
                    new FunctionDefinition{ Context = context },
                }
            };
        }

        public override NamespaceBody VisitNamespaceDefinition([NotNull] CPPCXParser.NamespaceDefinitionContext context)
        {
            return null;
        }

        protected override NamespaceBody AggregateResult(NamespaceBody aggregate, NamespaceBody nextResult)
        {
            if (nextResult == null) return aggregate;
            if (aggregate == null) return nextResult;
            aggregate.MergeFrom(nextResult);
            return aggregate;
        }
    }
}
