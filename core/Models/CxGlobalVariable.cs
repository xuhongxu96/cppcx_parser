using System;
using System.Collections.Generic;
using System.Text;

namespace cppcx.Core.Models
{
    public class CxGlobalVariable : IEquatable<CxGlobalVariable>
    {
        public CxTypedName TypedName { get; set; }
        public string InitValue { get; set; } = null;
        public bool IsExtern { get; set; } = false;

        public override bool Equals(object obj)
        {
            return Equals(obj as CxGlobalVariable);
        }

        public bool Equals(CxGlobalVariable other)
        {
            return other != null &&
                   EqualityComparer<CxTypedName>.Default.Equals(TypedName, other.TypedName);
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(TypedName);
        }

        public static bool operator ==(CxGlobalVariable left, CxGlobalVariable right)
        {
            return EqualityComparer<CxGlobalVariable>.Default.Equals(left, right);
        }

        public static bool operator !=(CxGlobalVariable left, CxGlobalVariable right)
        {
            return !(left == right);
        }
    }
}
